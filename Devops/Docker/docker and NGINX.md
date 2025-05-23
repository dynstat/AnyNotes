# Deploying Web Applications with Nginx and Docker on DigitalOcean

This guide provides practical examples of deploying various web applications (Flask, FastAPI, Django, and Node.js with Express) using the Nginx image as a reverse proxy. Both standalone Docker setups and Docker Compose configurations are covered. The deployment targets a DigitalOcean server with the domain `xyz.com` and specific port configurations.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Directory Structure](#directory-structure)
3. [Deploying Without Docker Compose](#deploying-without-docker-compose)
    - [1. Flask Application](#1-flask-application)
    - [2. FastAPI Application](#2-fastapi-application)
    - [3. Django Application](#3-django-application)
    - [4. Node.js (Express) Application](#4-nodejs-express-application)
4. [Deploying With Docker Compose](#deploying-with-docker-compose)
    - [1. Flask Application](#1-flask-application-1)
    - [2. FastAPI Application](#2-fastapi-application-1)
    - [3. Django Application](#3-django-application-1)
    - [4. Node.js (Express) Application](#4-nodejs-express-application-1)
5. [Nginx Configuration](#nginx-configuration)
6. [DNS Setup](#dns-setup)
7. [Conclusion](#conclusion)

---

## Prerequisites

- **DigitalOcean Droplet**: A server instance running Ubuntu 20.04 or later.
- **Domain Name**: `xyz.com` properly registered and pointing to your Droplet's IP.
- **Docker Installed**: Ensure Docker and Docker Compose are installed on your server.
- **Basic Knowledge**: Familiarity with Docker, Docker Compose, and Nginx.

---

## Directory Structure

Organize your project files as follows for consistency and maintainability:

```
project-root/
├── flask-app/
│   ├── app.py
│   ├── requirements.txt
│   └── Dockerfile
├── fastapi-app/
│   ├── main.py
│   ├── requirements.txt
│   └── Dockerfile
├── django-app/
│   ├── manage.py
│   ├── requirements.txt
│   ├── Dockerfile
│   └── myproject/
│       ├── settings.py
│       └── urls.py
├── nodejs-app/
│   ├── app.js
│   ├── package.json
│   └── Dockerfile
└── nginx/
    ├── default.conf
    └── Dockerfile
```

---

## Deploying Without Docker Compose

### 1. Flask Application

#### `flask-app/Dockerfile`

```dockerfile:flask-app/Dockerfile
# Use the official Python image.
# https://hub.docker.com/_/python
FROM python:3.10-slim

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

# Set work directory
WORKDIR /app

# Install dependencies
COPY requirements.txt .
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Copy project
COPY . .

# Expose the port Flask is running on
EXPOSE 5000

# Run the application
CMD ["python", "app.py"]
```

#### `flask-app/app.py`

```python:flask-app/app.py
from flask import Flask

app = Flask(__name__)

@app.route('/')
def hello():
    return "Hello from Flask!"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```

#### `flask-app/requirements.txt`

```plaintext:flask-app/requirements.txt
Flask==2.2.5
```

#### Building and Running the Flask Container

1. **Build the Docker Image:**

    ```bash
    docker build -t flask-app ./flask-app
    ```

    - `-t flask-app`: Tags the image as `flask-app`.
    - `./flask-app`: Specifies the build context.

2. **Run the Container:**

    ```bash
    docker run -d --name flask-container -p 5000:5000 flask-app
    ```

    - `-d`: Runs the container in detached mode.
    - `--name flask-container`: Names the container `flask-container`.
    - `-p 5000:5000`: Maps host port `5000` to container port `5000`.
    - `flask-app`: Specifies the image to run.

---

### 2. FastAPI Application

#### `fastapi-app/Dockerfile`

```dockerfile:fastapi-app/Dockerfile
# Use the official Python image.
FROM python:3.10-slim

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

# Set work directory
WORKDIR /app

# Install dependencies
COPY requirements.txt .
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Copy project
COPY . .

# Expose the port FastAPI is running on
EXPOSE 8000

# Run the application using Uvicorn
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
```

#### `fastapi-app/main.py`

```python:fastapi-app/main.py
from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World from FastAPI"}
```

#### `fastapi-app/requirements.txt`

```plaintext:fastapi-app/requirements.txt
fastapi==0.95.1
uvicorn==0.22.0
```

#### Building and Running the FastAPI Container

1. **Build the Docker Image:**

    ```bash
    docker build -t fastapi-app ./fastapi-app
    ```

2. **Run the Container:**

    ```bash
    docker run -d --name fastapi-container -p 8000:8000 fastapi-app
    ```

---

### 3. Django Application

#### `django-app/Dockerfile`

```dockerfile:django-app/Dockerfile
# Use the official Python image.
FROM python:3.10-slim

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

# Set work directory
WORKDIR /app

# Install dependencies
COPY requirements.txt .
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Copy project
COPY . .

# Collect static files
RUN python manage.py collectstatic --noinput

# Expose the port Django is running on
EXPOSE 8000

# Run migrations and start the server
CMD ["sh", "-c", "python manage.py migrate && python manage.py runserver 0.0.0.0:8000"]
```

#### `django-app/requirements.txt`

```plaintext:django-app/requirements.txt
Django==4.2.5
gunicorn==20.1.0
```

#### `django-app/myproject/settings.py` (Partial Configuration)

```python:django-app/myproject/settings.py
import os

# ... other settings ...

ALLOWED_HOSTS = ['*']  # Update this in production

# Static files (CSS, JavaScript, Images)
STATIC_URL = '/static/'
STATIC_ROOT = os.path.join(BASE_DIR, 'static/')
```

#### `django-app/myproject/urls.py`

```python:django-app/myproject/urls.py
from django.contrib import admin
from django.urls import path

urlpatterns = [
    path('admin/', admin.site.urls),
]
```

#### Building and Running the Django Container

1. **Build the Docker Image:**

    ```bash
    docker build -t django-app ./django-app
    ```

2. **Run the Container:**

    ```bash
    docker run -d --name django-container -p 8001:8000 django-app
    ```

    - Note: Using a different host port (`8001`) to avoid conflict with FastAPI.

---

### 4. Node.js (Express) Application

#### `nodejs-app/Dockerfile`

```dockerfile:nodejs-app/Dockerfile
# Use the official Node.js image.
FROM node:18-alpine

# Set working directory
WORKDIR /app

# Install app dependencies
COPY package.json .
RUN npm install

# Copy app source code
COPY . .

# Expose the port Express is running on
EXPOSE 3000

# Start the application
CMD ["node", "app.js"]
```

#### `nodejs-app/package.json`

```json:nodejs-app/package.json
{
  "name": "nodejs-app",
  "version": "1.0.0",
  "description": "Node.js Express application",
  "main": "app.js",
  "scripts": {
    "start": "node app.js"
  },
  "dependencies": {
    "express": "^4.18.2"
  }
}
```

#### `nodejs-app/app.js`

```javascript:nodejs-app/app.js
const express = require('express');
const app = express();
const port = 3000;

app.get('/', (req, res) => {
  res.send('Hello from Node.js Express!');
});

app.listen(port, () => {
  console.log(`Express app listening at http://0.0.0.0:${port}`);
});
```

#### Building and Running the Node.js Container

1. **Build the Docker Image:**

    ```bash
    docker build -t nodejs-app ./nodejs-app
    ```

2. **Run the Container:**

    ```bash
    docker run -d --name nodejs-container -p 3000:3000 nodejs-app
    ```

---

## Deploying With Docker Compose

Docker Compose simplifies managing multi-container Docker applications. Below are examples for each application type integrated with Nginx.

### 1. Flask Application

#### `docker-compose.yaml`

```yaml:docker-compose.yml
version: '3.8'

services:
  flask:
    build:
      context: ./flask-app
      dockerfile: Dockerfile
    container_name: flask-container
    ports:
      - "5000:5000"
    networks:
      - webnet

  nginx:
    image: nginx:latest
    container_name: nginx-flask
    ports:
      - "80:80"
    volumes:
      - ./nginx/default.conf:/etc/nginx/conf.d/default.conf
    depends_on:
      - flask
    networks:
      - webnet

networks:
  webnet:
```

#### `nginx/default.conf`

```nginx:nginx/default.conf
server {
    listen 80;
    server_name xyz.com;

    location / {
        proxy_pass http://flask:5000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

#### Running the Docker Compose Setup

1. **Navigate to the Project Root:**

    ```bash
    cd project-root
    ```

2. **Start the Services:**

    ```bash
    docker-compose up -d
    ```

    - `-d`: Runs containers in detached mode.

3. **Access the Application:**

    - Navigate to `http://xyz.com` in your browser.

---

### 2. FastAPI Application

#### `docker-compose.yml`

```yaml:docker-compose.yml
version: '3.8'

services:
  fastapi:
    build:
      context: ./fastapi-app
      dockerfile: Dockerfile
    container_name: fastapi-container
    ports:
      - "8000:8000"
    networks:
      - webnet

  nginx:
    image: nginx:latest
    container_name: nginx-fastapi
    ports:
      - "80:80"
    volumes:
      - ./nginx/default-fastapi.conf:/etc/nginx/conf.d/default.conf
    depends_on:
      - fastapi
    networks:
      - webnet

networks:
  webnet:
```

#### `nginx/default-fastapi.conf`

```nginx:nginx/default-fastapi.conf
server {
    listen 80;
    server_name xyz.com;

    location / {
        proxy_pass http://fastapi:8000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

#### Running the Docker Compose Setup

1. **Navigate to the Project Root:**

    ```bash
    cd project-root
    ```

2. **Start the Services:**

    ```bash
    docker-compose up -d
    ```

3. **Access the Application:**

    - Navigate to `http://xyz.com` in your browser.

---

### 3. Django Application

#### `docker-compose.yml`

```yaml:docker-compose.yml
version: '3.8'

services:
  django:
    build:
      context: ./django-app
      dockerfile: Dockerfile
    container_name: django-container
    ports:
      - "8001:8000"
    volumes:
      - ./django-app:/app
    networks:
      - webnet

  nginx:
    image: nginx:latest
    container_name: nginx-django
    ports:
      - "80:80"
    volumes:
      - ./nginx/default-django.conf:/etc/nginx/conf.d/default.conf
    depends_on:
      - django
    networks:
      - webnet

networks:
  webnet:
```

#### `nginx/default-django.conf`

```nginx:nginx/default-django.conf
server {
    listen 80;
    server_name xyz.com;

    location / {
        proxy_pass http://django:8000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }

    location /static/ {
        alias /app/static/;
    }
}
```

#### Running the Docker Compose Setup

1. **Navigate to the Project Root:**

    ```bash
    cd project-root
    ```

2. **Start the Services:**

    ```bash
    docker-compose up -d
    ```

3. **Apply Migrations and Collect Static Files:**

    ```bash
    docker exec django-container python manage.py migrate
    docker exec django-container python manage.py collectstatic --noinput
    ```

4. **Access the Application:**

    - Navigate to `http://xyz.com` in your browser.

---

### 4. Node.js (Express) Application

#### `docker-compose.yml`

```yaml:docker-compose.yml
version: '3.8'

services:
  nodejs:
    build:
      context: ./nodejs-app
      dockerfile: Dockerfile
    container_name: nodejs-container
    ports:
      - "3000:3000"
    networks:
      - webnet

  nginx:
    image: nginx:latest
    container_name: nginx-nodejs
    ports:
      - "80:80"
    volumes:
      - ./nginx/default-nodejs.conf:/etc/nginx/conf.d/default.conf
    depends_on:
      - nodejs
    networks:
      - webnet

networks:
  webnet:
```

#### `nginx/default-nodejs.conf`

```nginx:nginx/default-nodejs.conf
server {
    listen 80;
    server_name xyz.com;

    location / {
        proxy_pass http://nodejs:3000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

#### Running the Docker Compose Setup

1. **Navigate to the Project Root:**

    ```bash
    cd project-root
    ```

2. **Start the Services:**

    ```bash
    docker-compose up -d
    ```

3. **Access the Application:**

    - Navigate to `http://xyz.com` in your browser.

---

## Nginx Configuration

Nginx acts as a reverse proxy, forwarding incoming traffic to the appropriate application containers based on the configured server blocks.

### Common Nginx Configuration Options

- **`server_name`**: Specifies the domain name.
- **`proxy_pass`**: Defines the protocol and address of the proxied server.
- **`proxy_set_header`**: Sets the headers to pass to the proxied server.

#### Example: Nginx Default Configuration

```nginx:nginx/default.conf
server {
    listen 80;
    server_name xyz.com;

    location / {
        proxy_pass http://app_service:app_port;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

- Replace `app_service` with the Docker service name (e.g., `flask`) and `app_port` with the application's port (e.g., `5000`).

---

## DNS Setup

To ensure your domain `xyz.com` points to your DigitalOcean Droplet:

1. **Access Your Domain Registrar's Dashboard:**
    - Log in to the platform where you registered `xyz.com`.

2. **Set Up an A Record:**
    - **Host**: `@`
    - **Points to**: `<Droplet_IP_Address>`
    - **TTL**: Default or as desired

3. **Set Up a CNAME Record for `www` (Optional):**
    - **Host**: `www`
    - **Points to**: `xyz.com`
    - **TTL**: Default or as desired

4. **Propagation:**
    - DNS changes can take up to 24-48 hours to propagate globally, but usually complete within a few hours.

---

## Conclusion

Deploying web applications with Docker and Nginx on DigitalOcean provides a scalable and maintainable architecture. Whether you're using Flask, FastAPI, Django, or Node.js with Express, integrating Nginx as a reverse proxy ensures efficient handling of client requests. Docker Compose further streamlines the management of multi-container setups, simplifying deployment and scaling processes.

Remember to:

- **Secure Your Applications**: Implement SSL/TLS certificates using tools like Let's Encrypt.
- **Monitor Your Services**: Use monitoring tools to keep track of your application's performance and health.
- **Manage Environment Variables**: Securely handle sensitive information using Docker secrets or environment variable management tools.


