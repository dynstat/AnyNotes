Should work on macOS, Linux, and Windows.

``` cpp
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <map>
#include <set>
#include <json/json.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;

class WebSocketServer {
private:
    server m_server;
    std::map<std::string, con_list> m_topics;
    std::map<websocketpp::connection_hdl, std::string, std::owner_less<websocketpp::connection_hdl>> m_connections;

    const size_t MAX_MESSAGE_SIZE = 1024 * 1024; // 1 MB

    bool authenticate(const std::string& token) {
        // Implement your authentication logic here
        return token == "secret_token";
    }

    void on_open(websocketpp::connection_hdl hdl) {
        m_connections[hdl] = "";
        std::cout << "New connection opened" << std::endl;
    }

    void on_close(websocketpp::connection_hdl hdl) {
        std::string user_id = m_connections[hdl];
        m_connections.erase(hdl);

        // Remove connection from all subscribed topics
        for (auto& topic : m_topics) {
            topic.second.erase(hdl);
        }

        std::cout << "Connection closed for user: " << user_id << std::endl;
    }

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        if (msg->get_payload().size() > MAX_MESSAGE_SIZE) {
            send_error(hdl, "Message too large");
            return;
        }

        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(msg->get_payload(), root)) {
            send_error(hdl, "Invalid JSON");
            return;
        }

        if (m_connections[hdl].empty()) {
            // Not authenticated yet
            if (root.isMember("token") && authenticate(root["token"].asString())) {
                m_connections[hdl] = root["user_id"].asString();
                std::cout << "User authenticated: " << m_connections[hdl] << std::endl;
            } else {
                send_error(hdl, "Authentication failed");
                m_server.close(hdl, websocketpp::close::status::policy_violation, "Authentication failed");
            }
        } else if (root.isMember("subscribe")) {
            // Handle subscription
            for (const auto& topic : root["subscribe"]) {
                m_topics[topic.asString()].insert(hdl);
            }
        } else if (root.isMember("topic") && root.isMember("content")) {
            // Broadcast message
            broadcast(root["topic"].asString(), root["content"].asString(), hdl);
        }
    }

    void broadcast(const std::string& topic, const std::string& message, websocketpp::connection_hdl exclude) {
        Json::Value root;
        root["topic"] = topic;
        root["content"] = message;
        root["user_id"] = m_connections[exclude];

        std::string payload = Json::FastWriter().write(root);

        for (const auto& client : m_topics[topic]) {
            if (client.lock() != exclude.lock()) {
                m_server.send(client, payload, websocketpp::frame::opcode::text);
            }
        }
    }

    void send_error(websocketpp::connection_hdl hdl, const std::string& error) {
        Json::Value root;
        root["error"] = error;
        m_server.send(hdl, Json::FastWriter().write(root), websocketpp::frame::opcode::text);
    }

public:
    WebSocketServer() {
        m_server.set_access_channels(websocketpp::log::alevel::all);
        m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        m_server.init_asio();

        m_server.set_open_handler(std::bind(&WebSocketServer::on_open, this, std::placeholders::_1));
        m_server.set_close_handler(std::bind(&WebSocketServer::on_close, this, std::placeholders::_1));
        m_server.set_message_handler(std::bind(&WebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    }

    void run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        std::cout << "WebSocket server running on port " << port << std::endl;
        m_server.run();
    }
};

int main() {
    WebSocketServer server;
    server.run(8765);
    return 0;
}
```

Now, here's the complete client code:

```cpp
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <json/json.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef websocketpp::client<websocketpp::config::asio_client> client;

class WebSocketClient {
private:
    client m_client;
    websocketpp::connection_hdl m_hdl;
    std::string m_uri;

    void on_open(websocketpp::connection_hdl hdl) {
        m_hdl = hdl;
        std::cout << "Connection opened" << std::endl;

        // Authenticate
        Json::Value auth;
        auth["token"] = "secret_token";
        auth["user_id"] = "user123";
        send_message(Json::FastWriter().write(auth));

        // Subscribe to topics
        Json::Value sub;
        sub["subscribe"] = Json::Value(Json::arrayValue);
        sub["subscribe"].append("topic1");
        sub["subscribe"].append("topic2");
        send_message(Json::FastWriter().write(sub));
    }

    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
        std::cout << "Received: " << msg->get_payload() << std::endl;

        Json::Value root;
        Json::Reader reader;
        if (reader.parse(msg->get_payload(), root)) {
            if (root.isMember("content") && root["content"].asString() == "ping") {
                Json::Value pong;
                pong["topic"] = "topic1";
                pong["content"] = "pong";
                send_message(Json::FastWriter().write(pong));
            }
        }
    }

    void on_close(websocketpp::connection_hdl hdl) {
        std::cout << "Connection closed" << std::endl;
    }

public:
    WebSocketClient(const std::string& uri) : m_uri(uri) {
        m_client.set_access_channels(websocketpp::log::alevel::all);
        m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

        m_client.init_asio();

        m_client.set_open_handler(std::bind(&WebSocketClient::on_open, this, std::placeholders::_1));
        m_client.set_message_handler(std::bind(&WebSocketClient::on_message, this, std::placeholders::_1, std::placeholders::_2));
        m_client.set_close_handler(std::bind(&WebSocketClient::on_close, this, std::placeholders::_1));
    }

    void run() {
        websocketpp::lib::error_code ec;
        client::connection_ptr con = m_client.get_connection(m_uri, ec);
        if (ec) {
            std::cout << "Connect initialization error: " << ec.message() << std::endl;
            return;
        }

        m_client.connect(con);
        m_client.run();
    }

    void send_message(const std::string& message) {
        m_client.send(m_hdl, message, websocketpp::frame::opcode::text);
    }
};

int main() {
    WebSocketClient client("ws://localhost:8765");
    client.run();
    return 0;
}
```

To compile these programs on different platforms, follow these instructions:

### macOS and Linux

1. Install dependencies:
   - On macOS: `brew install boost jsoncpp`
   - On Linux: `sudo apt-get install libboost-all-dev libjsoncpp-dev`

2. Clone the WebSocket++ library:
   ```
   git clone https://github.com/zaphoyd/websocketpp.git
   ```

3. Compile (adjust paths as needed):
   ```
   g++ -std=c++11 server.cpp -o server -I/path/to/websocketpp -I/usr/include/jsoncpp -ljsoncpp -lboost_system -lpthread
   g++ -std=c++11 client.cpp -o client -I/path/to/websocketpp -I/usr/include/jsoncpp -ljsoncpp -lboost_system -lpthread
   ```

### Windows

1. Install vcpkg (if not already installed):
   ```
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

2. Install dependencies:
   ```
   .\vcpkg install boost-asio jsoncpp websocketpp
   ```

3. Open the project in Visual Studio and add these lines to your project's Properties -> C/C++ -> Additional Include Directories:
   ```
   C:\path\to\vcpkg\installed\x64-windows\include
   ```

4. Add this line to your project's Properties -> Linker -> Additional Library Directories:
   ```
   C:\path\to\vcpkg\installed\x64-windows\lib
   ```

5. Add these libraries to your project's Properties -> Linker -> Input -> Additional Dependencies:
   ```
   jsoncpp.lib
   ```

6. Build the project in Visual Studio.
