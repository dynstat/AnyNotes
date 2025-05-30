# Secure Deployment of FastAPI Web App on Ubuntu VPS with Docker, Docker Compose, and Nginx

This comprehensive guide will walk you through deploying a FastAPI web application on an Ubuntu VPS using Docker, Docker Compose, and Nginx. Emphasis is placed on security and optimization best practices to ensure a robust and efficient deployment.

## Table of Contents

1. [Initial Server Setup](#1-initial-server-setup)
2. [Create a Non-Root User](#2-create-a-non-root-user)
3. [Secure SSH Access](#3-secure-ssh-access)
4. [Set Up Firewall with UFW](#4-set-up-firewall-with-ufw)
5. [Install Docker and Docker Compose](#5-install-docker-and-docker-compose)
6. [Configure Docker Daemon Security](#6-configure-docker-daemon-security)
7. [Set Up Project Structure](#7-set-up-project-structure)
8. [Create FastAPI Application](#8-create-fastapi-application)
9. [Write Dockerfile for FastAPI](#9-write-dockerfile-for-fastapi)
10. [Create Docker Compose Configuration](#10-create-docker-compose-configuration)
11. [Configure Nginx as Reverse Proxy](#11-configure-nginx-as-reverse-proxy)
12. [Set Up SSL with Let's Encrypt](#12-set-up-ssl-with-lets-encrypt)
13. [Optimize Server Performance](#13-optimize-server-performance)
14. [Deploy the Application](#14-deploy-the-application)
15. [Post-Deployment Security Measures](#15-post-deployment-security-measures)

---

## 1. Initial Server Setup

Begin by updating and upgrading your Ubuntu system to ensure all packages are up-to-date.

```bash
# Update package lists and upgrade installed packages
apt update && apt upgrade -y
```

**Explanation:**
- `apt update`: Refreshes the list of available packages and their versions.
- `apt upgrade -y`: Upgrades all installed packages to their latest versions automatically answering 'yes' to prompts.

---

## 2. Create a Non-Root User

Operating as the root user poses significant security risks. Create a dedicated user with sudo privileges.

```bash
# Create a new user named 'webadmin'
sudo adduser webadmin

# Grant 'webadmin' sudo privileges
sudo usermod -aG sudo webadmin
```

**Explanation:**
- `adduser webadmin`: Adds a new user named `webadmin`.
- `usermod -aG sudo webadmin`: Appends the `webadmin` user to the `sudo` group, granting administrative privileges.

---

## 3. Secure SSH Access

Enhance SSH security by disabling root login and using SSH keys.

```bash
# Switch to the new user
su - webadmin

# Generate SSH keys on your local machine
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"

# Copy the public key to the server
ssh-copy-id webadmin@your_server_ip
```

**Explanation:**
- `ssh-keygen`: Generates a new SSH key pair.
- `ssh-copy-id`: Copies the public SSH key to the server for passwordless authentication.

```bash
# Edit SSH configuration
sudo nano /etc/ssh/sshd_config
```

**Modify the following settings:**
```ini
PermitRootLogin no
PasswordAuthentication no
```

**Restart SSH service:**
```bash
sudo systemctl restart sshd
```

**Explanation:**
- `PermitRootLogin no`: Disables SSH login for the root user.
- `PasswordAuthentication no`: Disables password-based SSH authentication, enforcing key-based authentication.

---

## 4. Set Up Firewall with UFW

Configure UFW (Uncomplicated Firewall) to allow only necessary ports.

```bash
# Install UFW if not already installed
sudo apt install ufw -y

# Allow SSH
sudo ufw allow OpenSSH

# Allow HTTP and HTTPS
sudo ufw allow 'Nginx Full'

# Enable UFW
sudo ufw enable
```

**Explanation:**
- `sudo ufw allow OpenSSH`: Allows SSH traffic.
- `sudo ufw allow 'Nginx Full'`: Allows HTTP (port 80) and HTTPS (port 443) traffic.
- `sudo ufw enable`: Activates the firewall with the defined rules.

---

## 5. Install Docker and Docker Compose

Install Docker Engine and Docker Compose securely.

```bash
# Update package database
sudo apt update

# Install prerequisites
sudo apt install apt-transport-https ca-certificates curl gnupg lsb-release -y

# Add Docker’s official GPG key
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

# Set up the stable repository
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] \
  https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Update package database with Docker packages
sudo apt update

# Install Docker Engine
sudo apt install docker-ce docker-ce-cli containerd.io -y

# Verify Docker installation
sudo docker run hello-world
```

**Explanation:**
- Adds Docker’s official GPG key and repository for secure installation.
- Installs Docker Engine components.
- Runs a test Docker container to verify the installation.

```bash
# Install Docker Compose
sudo curl -L "https://github.com/docker/compose/releases/download/v2.20.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose

# Apply executable permissions
sudo chmod +x /usr/local/bin/docker-compose

# Verify Docker Compose installation
docker-compose --version
```

**Explanation:**
- Downloads Docker Compose binary and places it in `/usr/local/bin`.
- Grants execute permissions.
- Verifies the installation by checking the version.

---

## 6. Configure Docker Daemon Security

Enhance Docker security by configuring the Docker daemon.

```bash
# Create Docker daemon configuration directory
sudo mkdir /etc/docker

# Create daemon.json with secure settings
sudo nano /etc/docker/daemon.json
```

**`/etc/docker/daemon.json`:**
```json
{
  "auto-restart": "unless-stopped",
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "10m",
    "max-file": "3"
  },
  "userns-remap": "default",
  "no-new-privileges": true,
  "default-ulimits": {
    "nofile": {
      "Name": "nofile",
      "Hard": 64000,
      "Soft": 64000
    }
  }
}
```

**Explanation:**
- `auto-restart`: Ensures containers restart unless explicitly stopped.
- `log-driver` and `log-opts`: Limits log file sizes to prevent disk space exhaustion.
- `userns-remap`: Enables user namespace remapping for isolation.
- `no-new-privileges`: Prevents containers from gaining new privileges.
- `default-ulimits`: Sets default limits for file descriptors to enhance performance.

```bash
# Restart Docker to apply changes
sudo systemctl restart docker
```

---

## 7. Set Up Project Structure

Organize your project directories for maintainability and security.

```bash
# Create project directory
mkdir -p ~/myapp/{app,nginx,certbot}

# Navigate to project directory
cd ~/myapp
```

**Explanation:**
- `app`: Contains FastAPI application code.
- `nginx`: Holds Nginx configuration files.
- `certbot`: Used for SSL certificate management with Let's Encrypt.

---

## 8. Create FastAPI Application

Develop a simple FastAPI application.

```python:~/myapp/app/main.py
from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World"}
```

**Explanation:**
- Defines a FastAPI instance with a single route that returns a JSON response.
- Save this file as `main.py` within the `app` directory.

```bash
# Create requirements.txt
echo "fastapi
uvicorn[standard]" > ~/myapp/app/requirements.txt
```

**Explanation:**
- Lists necessary Python packages for the FastAPI application.

---

## 9. Write Dockerfile for FastAPI

Containerize the FastAPI application with a secure and optimized Dockerfile.

### ~/myapp/Dockerfile

```dockerfile
# Use official Python runtime as a parent image
FROM python:3.11-slim

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

# Create a non-root user
RUN useradd -m appuser

# Set working directory
WORKDIR /app

# Install dependencies
COPY ./app/requirements.txt .
RUN pip install --upgrade pip
RUN pip install --no-cache-dir -r requirements.txt

# Copy application code
COPY ./app /app

# Change ownership to non-root user
RUN chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Expose port
EXPOSE 8000

# Run the application
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
```

**Explanation:**
- **Base Image:** Uses Python 3.11 slim image for a lightweight container.
- **Environment Variables:**
  - `PYTHONDONTWRITEBYTECODE`: Prevents Python from writing `.pyc` files.
  - `PYTHONUNBUFFERED`: Ensures logs are output immediately.
- **Non-Root User:** Creates `appuser` to run the application, enhancing security.
- **Working Directory:** Sets `/app` as the working directory inside the container.
- **Dependencies Installation:** Installs Python dependencies without caching to reduce image size.
- **File Ownership:** Assigns ownership of application files to `appuser`.
- **Port Exposure:** Exposes port `8000` for the FastAPI app.
- **Command:** Starts the FastAPI application using Uvicorn.

---

## 10. Create Docker Compose Configuration

Define services and manage multi-container deployments with Docker Compose.

### ~/myapp/docker-compose.yml

```yaml
version: '3.8'

services:
  web:
    build: .
    restart: unless-stopped
    env_file:
      - .env
    networks:
      - app_network
    depends_on:
      - db

  nginx:
    image: nginx:1.23-alpine
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - ./nginx/nginx.conf:/etc/nginx/nginx.conf:ro
      - ./nginx/conf.d:/etc/nginx/conf.d:ro
      - ./certbot/conf:/etc/letsencrypt
      - ./certbot/www:/var/www/certbot
    depends_on:
      - web
    networks:
      - app_network

  certbot:
    image: certbot/certbot
    volumes:
      - ./certbot/conf:/etc/letsencrypt
      - ./certbot/www:/var/www/certbot
    entrypoint: "/bin/sh -c 'trap exit TERM; while :; do certbot renew; sleep 12h & wait $${!}; done;'"
    networks:
      - app_network

networks:
  app_network:
    driver: bridge
```

**Explanation:**
- **web Service:**
  - Builds the FastAPI application from the Dockerfile.
  - Restarts automatically unless explicitly stopped.
  - Loads environment variables from `.env` file.
  - Connects to `app_network` for isolated networking.
  - Depends on the `db` service (if applicable; adjust as needed).

- **nginx Service:**
  - Uses the lightweight Nginx Alpine image.
  - Maps host ports 80 and 443 to container ports.
  - Mounts Nginx configuration files and SSL directories.
  - Depends on the `web` service to ensure it starts after the web app.
  - Connects to `app_network`.

- **certbot Service:**
  - Utilizes Certbot for SSL certificate management.
  - Mounts directories for SSL certificates and webroot.
  - Runs a loop to renew certificates every 12 hours.
  - Connects to `app_network`.

- **Networks:**
  - Defines a bridge network `app_network` for inter-service communication.

> **Note:** If your application requires a database, include a `db` service in the `docker-compose.yml`. Adjust dependencies accordingly.

---

## 11. Configure Nginx as Reverse Proxy

Set up Nginx to forward requests to the FastAPI application securely.

### ~/myapp/nginx/nginx.conf

```nginx
user nginx;
worker_processes auto;
error_log /var/log/nginx/error.log warn;
pid /var/run/nginx.pid;

events {
    worker_connections 1024;
}

http {
    include /etc/nginx/mime.types;
    default_type application/octet-stream;

    log_format main '$remote_addr - $remote_user [$time_local] "$request" '
                    '$status $body_bytes_sent "$http_referer" '
                    '"$http_user_agent" "$http_x_forwarded_for"';

    access_log /var/log/nginx/access.log main;
    sendfile on;
    tcp_nopush on;
    tcp_nodelay on;
    keepalive_timeout 65;
    types_hash_max_size 2048;

    server_tokens off;
    client_max_body_size 10M;

    # Security Headers
    add_header X-Frame-Options "DENY" always;
    add_header X-Content-Type-Options "nosniff" always;
    add_header X-XSS-Protection "1; mode=block" always;
    add_header Strict-Transport-Security "max-age=63072000; includeSubDomains; preload" always;

    include /etc/nginx/conf.d/*.conf;
}
```

**Explanation:**
- **Worker Configuration:**
  - `worker_processes auto`: Adjusts the number of worker processes based on available CPU cores.
  - `worker_connections 1024`: Sets the maximum number of simultaneous connections per worker.

- **HTTP Settings:**
  - Enables essential MIME types and default types.
  - Configures access and error logging.
  - Optimizes network settings with `sendfile`, `tcp_nopush`, `tcp_nodelay`, and `keepalive_timeout`.
  - Enhances security by disabling server tokens and limiting client request sizes.

- **Security Headers:**
  - `X-Frame-Options`: Prevents clickjacking by disallowing the page to be framed.
  - `X-Content-Type-Options`: Stops browsers from MIME-sniffing a response away from the declared content-type.
  - `X-XSS-Protection`: Enables Cross-site scripting (XSS) filter in browsers.
  - `Strict-Transport-Security`: Enforces secure (HTTP over SSL/TLS) connections to the server.

### ~/myapp/nginx/conf.d/app.conf

```nginx
server {
    listen 80;
    server_name your_domain.com;

    location /.well-known/acme-challenge/ {
        root /var/www/certbot;
    }

    location / {
        return 301 https://$host$request_uri;
    }
}

server {
    listen 443 ssl;
    server_name your_domain.com;

    ssl_certificate /etc/letsencrypt/live/your_domain.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/your_domain.com/privkey.pem;
    ssl_trusted_certificate /etc/letsencrypt/live/your_domain.com/chain.pem;

    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_prefer_server_ciphers on;
    ssl_ciphers HIGH:!aNULL:!MD5;

    ssl_session_cache shared:SSL:10m;
    ssl_session_timeout 10m;

    # Security Headers (Duplicated for SSL server)
    add_header X-Frame-Options "DENY" always;
    add_header X-Content-Type-Options "nosniff" always;
    add_header X-XSS-Protection "1; mode=block" always;
    add_header Strict-Transport-Security "max-age=63072000; includeSubDomains; preload" always;

    location / {
        proxy_pass http://web:8000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
        proxy_redirect off;
        proxy_http_version 1.1;
        proxy_set_header Connection "";
        proxy_buffering off;
    }
}
```

**Explanation:**
- **HTTP Server Block:**
  - Listens on port 80.
  - Handles ACME challenges for Let's Encrypt.
  - Redirects all other traffic to HTTPS to enforce secure connections.

- **HTTPS Server Block:**
  - Listens on port 443 with SSL certificates.
  - Specifies strong SSL protocols and ciphers.
  - Configures SSL session settings for performance.
  - Reiterates security headers for SSL connections.
  - Sets up reverse proxy to forward requests to the FastAPI application running on the `web` service.
  - Ensures headers like `Host`, `X-Real-IP`, and `X-Forwarded-For` are correctly forwarded.
  - Disables proxy buffering to improve real-time performance.

> **Note:** Replace `your_domain.com` with your actual domain name in the configuration files.

---

## 12. Set Up SSL with Let's Encrypt

Automate SSL certificate issuance and renewal using Certbot.

```bash
# Install Certbot and Nginx plugin
sudo apt install certbot python3-certbot-nginx -y
```

**Explanation:**
- Installs Certbot and the Nginx plugin for automated SSL certificate management.

```bash
# Obtain SSL certificate
sudo certbot --nginx -d your_domain.com -d www.your_domain.com
```

**Explanation:**
- Requests SSL certificates for `your_domain.com` and `www.your_domain.com`.
- Automatically configures Nginx to use the obtained certificates.

**Automatic Renewal:**
The `certbot` service defined in `docker-compose.yml` handles automatic renewal every 12 hours.

---

## 13. Optimize Server Performance

Enhance server performance through system tuning and Docker optimizations.

```bash
# Install essential tools
sudo apt install htop vim -y
```

**Explanation:**
- `htop`: Interactive process viewer for monitoring system performance.
- `vim`: Text editor for file manipulation.

```bash
# Tune system parameters for performance
sudo nano /etc/sysctl.conf
```

**Add the following lines:**
```ini
net.core.somaxconn=1024
net.ipv4.tcp_tw_reuse=1
net.ipv4.ip_local_port_range=1024 65535
```

**Apply the changes:**
```bash
sudo sysctl -p
```

**Explanation:**
- `net.core.somaxconn`: Increases the maximum number of connections that can be queued.
- `net.ipv4.tcp_tw_reuse`: Allows reuse of TIME_WAIT sockets for new connections.
- `net.ipv4.ip_local_port_range`: Expands the range of ephemeral ports.

```bash
# Limit Docker's resource usage (optional)
# Modify /etc/docker/daemon.json
sudo nano /etc/docker/daemon.json
```

**Add resource limits if desired:**
```json
{
  "default-ulimits": {
    "nofile": {
      "Name": "nofile",
      "Hard": 64000,
      "Soft": 64000
    }
  },
  "auto-restart": "unless-stopped",
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "10m",
    "max-file": "3"
  },
  "userns-remap": "default",
  "no-new-privileges": true
}
```

**Restart Docker:**
```bash
sudo systemctl restart docker
```

**Explanation:**
- Sets default ulimits and log options to prevent resource exhaustion.
- Ensures security by remapping user namespaces and preventing privilege escalation.

---

## 14. Deploy the Application

Build and run the Docker containers using Docker Compose.

```bash
# Navigate to project directory
cd ~/myapp

# Create .env file for environment variables
nano .env
```

**`.env:~/myapp/.env`:**
```env
# Secret key for FastAPI
SECRET_KEY=your_secret_key_here

# FastAPI settings
DEBUG=False
ALLOWED_HOSTS=your_domain.com,www.your_domain.com
```

**Explanation:**
- Stores sensitive information and configuration settings securely.
- Ensure `.env` is not exposed publicly and is included in `.gitignore` if using version control.

```bash
# Build and start containers in detached mode
docker-compose up -d --build
```

**Explanation:**
- `--build`: Forces rebuilding of images.
- `-d`: Runs containers in the background.

```bash
# Check container status
docker-compose ps
```

**Explanation:**
- Displays the status of running containers to verify successful deployment.

---

## 15. Post-Deployment Security Measures

Finalize security settings to protect your deployed application.

```bash
# Enable automatic security updates
sudo apt install unattended-upgrades -y
sudo dpkg-reconfigure --priority=low unattended-upgrades
```

**Explanation:**
- Installs and configures automatic security updates to keep the system patched against vulnerabilities.

```bash
# Install Fail2Ban to protect against brute-force attacks
sudo apt install fail2ban -y

# Create local configuration
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local

# Edit jail.local
sudo nano /etc/fail2ban/jail.local
```

**Configure SSH Protection:**
```ini
[sshd]
enabled = true
port = 22
filter = sshd
logpath = /var/log/auth.log
maxretry = 5
```

**Restart Fail2Ban:**
```bash
sudo systemctl restart fail2ban
```

**Explanation:**
- Protects SSH and other services from brute-force attacks by banning offending IPs after multiple failed attempts.

```bash
# Set up Docker Bench for Security
docker run -it --net host --pid host --userns host \
  --cap-add audit_control \
  -v /etc:/etc:ro \
  -v /usr/bin/docker-containerd:/usr/bin/docker-containerd:ro \
  -v /usr/bin/docker-runc:/usr/bin/docker-runc:ro \
  docker/docker-bench-security
```

**Explanation:**
- Runs Docker Bench for Security to scan Docker configurations against best practices.
- Reviews and addresses any highlighted security issues.

```bash
# Limit Docker container capabilities
# Edit docker-compose.yml to add security options
services:
  web:
    ...
    security_opt:
      - no-new-privileges:true
    cap_drop:
      - ALL
    read_only: true
```

**Explanation:**
- `no-new-privileges`: Prevents containers from gaining new privileges.
- `cap_drop`: Removes all Linux capabilities from the container.
- `read_only`: Mounts container's filesystem as read-only to prevent unauthorized modifications.

```yaml
services:
  web:
    build: .
    restart: unless-stopped
    env_file:
      - .env
    networks:
      - app_network
    depends_on:
      - db
    security_opt:
      - no-new-privileges:true
    cap_drop:
      - ALL
    read_only: true
```

**Explanation:**
- Enhances container security by restricting capabilities and enforcing read-only filesystems where possible.

---

# Conclusion

By following this guide, you've securely deployed a FastAPI web application on an Ubuntu VPS using Docker, Docker Compose, and Nginx. Key security and optimization practices implemented include:

- **User Management:** Operating with a non-root user and securing SSH access with key-based authentication.
- **Firewall Configuration:** Restricting network access to necessary ports using UFW.
- **Docker Security:** Configuring the Docker daemon with user namespaces, limiting resource usage, and running containers with minimal privileges.
- **Nginx Configuration:** Setting up Nginx as a reverse proxy with strong SSL/TLS settings and security headers.
- **SSL Automation:** Utilizing Certbot for automated SSL certificate management.
- **System Hardening:** Applying system-level optimizations and employing tools like Fail2Ban and Docker Bench for Security.

Regularly monitor your server, keep all components updated, and review security configurations to maintain a secure and efficient deployment environment.