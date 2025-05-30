I'll help you create a custom implementation similar to what Vercel does, but from scratch. This will help you understand the entire process.

### 1. First, let's create our FastAPI application

````python:app/main.py
from fastapi import FastAPI
from typing import Dict

app = FastAPI()

@app.get("/hello/{name}")
async def say_hello(name: str) -> Dict:
    return {"message": f"Hello, {name}!"}

@app.post("/calculate")
async def calculate(x: int, y: int) -> Dict:
    return {"sum": x + y, "product": x * y}
````

### 2. Create Our Custom Serverless Implementation

First, let's create the core components:

````python:serverless/core/request.py
from dataclasses import dataclass
from typing import Dict, Any, Optional
import json

@dataclass
class ServerlessRequest:
    path: str
    method: str
    headers: Dict[str, str]
    query_params: Dict[str, str]
    body: Optional[str] = None
    
    @classmethod
    def from_http_request(cls, raw_request: Dict[str, Any]) -> 'ServerlessRequest':
        """Convert raw HTTP request to ServerlessRequest"""
        return cls(
            path=raw_request.get('path', '/'),
            method=raw_request.get('method', 'GET').upper(),
            headers=raw_request.get('headers', {}),
            query_params=raw_request.get('query_params', {}),
            body=raw_request.get('body')
        )
    
    def to_asgi_scope(self) -> Dict[str, Any]:
        """Convert to ASGI scope dictionary"""
        return {
            'type': 'http',
            'asgi': {'version': '3.0'},
            'http_version': '1.1',
            'method': self.method,
            'scheme': 'http',
            'path': self.path,
            'query_string': '&'.join(f"{k}={v}" for k, v in self.query_params.items()).encode(),
            'headers': [[k.lower().encode(), v.encode()] for k, v in self.headers.items()]
        }
````

````python:serverless/core/response.py
from dataclasses import dataclass
from typing import Dict, Any, Optional
import json

@dataclass
class ServerlessResponse:
    status_code: int
    body: Any
    headers: Dict[str, str]
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert response to dictionary format"""
        return {
            'status_code': self.status_code,
            'body': json.dumps(self.body) if isinstance(self.body, (dict, list)) else str(self.body),
            'headers': self.headers
        }
````

### 3. Create the ASGI Bridge

````python:serverless/core/bridge.py
import asyncio
from typing import Dict, Any, Callable
from .request import ServerlessRequest
from .response import ServerlessResponse

class ASGIBridge:
    def __init__(self, app):
        self.app = app
    
    async def __call__(self, request: ServerlessRequest) -> ServerlessResponse:
        """Handle the request through ASGI application"""
        response_data = {
            'status_code': None,
            'headers': {},
            'body': b''
        }
        
        async def receive():
            """ASGI receive channel"""
            return {
                'type': 'http.request',
                'body': request.body.encode() if request.body else b'',
                'more_body': False
            }
        
        async def send(message: Dict[str, Any]):
            """ASGI send channel"""
            if message['type'] == 'http.response.start':
                response_data['status_code'] = message['status']
                response_data['headers'].update({
                    k.decode(): v.decode() 
                    for k, v in message['headers']
                })
            elif message['type'] == 'http.response.body':
                response_data['body'] += message['body']
        
        # Call the ASGI application
        await self.app(
            request.to_asgi_scope(),
            receive,
            send
        )
        
        # Convert response body to string/JSON
        try:
            body = response_data['body'].decode()
            body = json.loads(body)  # Try to parse JSON
        except (UnicodeDecodeError, json.JSONDecodeError):
            body = str(response_data['body'])
        
        return ServerlessResponse(
            status_code=response_data['status_code'],
            headers=response_data['headers'],
            body=body
        )
````

### 4. Create the Function Generator

````python:serverless/generator/function_generator.py
import os
import inspect
from typing import Dict, Any
import importlib.util

class FunctionGenerator:
    def __init__(self, app):
        self.app = app
        self.output_dir = "generated_functions"
    
    def generate_functions(self):
        """Generate serverless function files for each route"""
        os.makedirs(self.output_dir, exist_ok=True)
        
        for route in self.app.routes:
            if route.path == "/openapi.json":  # Skip OpenAPI routes
                continue
            
            # Create function file
            function_name = self._get_function_name(route.path)
            self._generate_function_file(function_name, route)
    
    def _get_function_name(self, path: str) -> str:
        """Convert path to valid function name"""
        return path.strip('/').replace('/', '_').replace('{', '').replace('}', '')
    
    def _generate_function_file(self, function_name: str, route):
        """Generate individual function file"""
        template = self._get_function_template(function_name, route)
        
        file_path = os.path.join(self.output_dir, f"{function_name}.py")
        with open(file_path, 'w') as f:
            f.write(template)
    
    def _get_function_template(self, function_name: str, route) -> str:
        return f'''
from serverless.core.request import ServerlessRequest
from serverless.core.bridge import ASGIBridge
from app.main import app

bridge = ASGIBridge(app)

async def handle(request_data):
    request = ServerlessRequest.from_http_request(request_data)
    response = await bridge(request)
    return response.to_dict()
'''
````

### 5. Create the Runtime Handler

````python:serverless/runtime/handler.py
import asyncio
import importlib.util
import sys
from typing import Dict, Any
from pathlib import Path

class RuntimeHandler:
    def __init__(self, functions_dir: str = "generated_functions"):
        self.functions_dir = Path(functions_dir)
    
    async def handle_request(self, request_data: Dict[str, Any]) -> Dict[str, Any]:
        """Handle incoming request by routing to appropriate function"""
        path = request_data.get('path', '/').strip('/')
        function_name = path.replace('/', '_')
        
        # Load the function module
        function_path = self.functions_dir / f"{function_name}.py"
        if not function_path.exists():
            return {
                'status_code': 404,
                'body': {'error': 'Function not found'},
                'headers': {'Content-Type': 'application/json'}
            }
        
        # Import the function
        spec = importlib.util.spec_from_file_location(
            function_name, 
            str(function_path)
        )
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        
        # Execute the function
        try:
            response = await module.handle(request_data)
            return response
        except Exception as e:
            return {
                'status_code': 500,
                'body': {'error': str(e)},
                'headers': {'Content-Type': 'application/json'}
            }
````

### 6. Create a Simple HTTP Server to Test

````python:server.py
import asyncio
from aiohttp import web
from serverless.runtime.handler import RuntimeHandler
from serverless.generator.function_generator import FunctionGenerator
from app.main import app

# Generate serverless functions
generator = FunctionGenerator(app)
generator.generate_functions()

# Create runtime handler
runtime = RuntimeHandler()

async def handle_request(request):
    """Handle incoming HTTP requests"""
    # Convert aiohttp request to our format
    request_data = {
        'path': request.path,
        'method': request.method,
        'headers': dict(request.headers),
        'query_params': dict(request.query),
        'body': await request.text() if request.can_read_body else None
    }
    
    # Process through our serverless runtime
    response = await runtime.handle_request(request_data)
    
    # Convert response back to aiohttp format
    return web.json_response(
        response['body'],
        status=response['status_code'],
        headers=response['headers']
    )

# Create and run the server
app = web.Application()
app.router.add_route('*', '/{tail:.*}', handle_request)

if __name__ == '__main__':
    web.run_app(app, port=8000)
````

### 7. Test the Implementation

Create a test script:

````python:test_local.py
import asyncio
import aiohttp
import json

async def test_endpoints():
    async with aiohttp.ClientSession() as session:
        # Test GET /hello/world
        async with session.get('http://localhost:8000/hello/world') as response:
            print("GET /hello/world:")
            print(await response.json())
            print()
        
        # Test POST /calculate
        data = {'x': 5, 'y': 3}
        async with session.post(
            'http://localhost:8000/calculate',
            json=data
        ) as response:
            print("POST /calculate:")
            print(await response.json())

if __name__ == "__main__":
    asyncio.run(test_endpoints())
````

### How to Use

1. First, generate the serverless functions:
```bash
python -c "from serverless.generator.function_generator import FunctionGenerator; from app.main import app; FunctionGenerator(app).generate_functions()"
```

2. Start the server:
```bash
python server.py
```

3. Run the tests:
```bash
python test_local.py
```

### Directory Structure

```
your_project/
├── app/
│   └── main.py              # FastAPI application
├── serverless/
│   ├── core/
│   │   ├── request.py       # Request handling
│   │   ├── response.py      # Response handling
│   │   └── bridge.py        # ASGI bridge
│   ├── generator/
│   │   └── function_generator.py  # Function generator
│   └── runtime/
│       └── handler.py       # Runtime handler
├── generated_functions/     # Generated serverless functions
├── server.py               # Test server
└── test_local.py          # Test script
```

This implementation:
1. Converts FastAPI routes to individual serverless functions
2. Handles requests without a continuous server
3. Maintains FastAPI's ASGI compatibility
4. Provides proper request/response handling

To make it truly serverless, you would:
1. Deploy generated functions to a cloud platform
2. Set up API Gateway or similar service
3. Configure proper routing
4. Handle scaling and concurrency

This is a basic implementation that demonstrates the core concepts. In a production environment, you'd need to add:
1. Error handling
2. Logging
3. Security measures
4. Performance optimizations
5. Proper state management
6. Connection pooling for databases