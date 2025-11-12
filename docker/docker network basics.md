# 🐳 **Communication in Docker: Containers & Networks**

At the core, **all communication in Docker happens via networks**.
By default, containers **don’t talk to each other unless you explicitly allow it**.

---

## 1. **Types of Communication**

### **1.1. Container ↔ Outside World (Host or Internet)**

* By default, containers started with the **default `bridge` network** can access the **outside internet**.
* Host ↔ Container communication:

  * Use **port mapping (`-p hostPort:containerPort`)**.

Example:

```bash
docker run -d -p 8080:80 nginx
```

* Container runs NGINX on port `80` internally.
* Host machine (and external users) can reach it at `http://localhost:8080`.

---

### **1.2. Container ↔ Container (Same Network)**

* Containers can communicate if they are in the **same Docker network**.
* Inside a user-defined bridge network, containers can **reach each other by name**.

Example:

```bash
docker network create mynet
docker run -d --name web --network mynet nginx
docker run -it --name client --network mynet alpine sh
```

Inside `client` container:

```sh
ping web   # resolves via Docker's embedded DNS
```

---

### **1.3. Restricting Communication**

* You can isolate containers by:

  * Putting them in **different networks**.
  * Using **firewall rules (iptables)** or **network policies**.
  * Using **`none` network** → container has **no network at all**.

Example:

```bash
docker run -it --network none alpine sh
# No internet, no container-to-container communication
```

---

### **1.4. Selected Containers Only (Groups)**

* Use **user-defined networks**.
* Containers in the same user-defined network can talk to each other.
* Containers in different networks **cannot**, unless connected manually.

Example:

```bash
docker network create frontend
docker network create backend

# Web app (in frontend)
docker run -d --name web --network frontend nginx

# Database (in backend)
docker run -d --name db --network backend mysql

# API (in both frontend and backend)
docker run -d --name api --network frontend --network backend myapp
```

Now:

* `web ↔ api` ✅ (frontend network)
* `api ↔ db` ✅ (backend network)
* `web ↔ db` ❌ (no shared network)

---

## 2. **Docker Networks**

| Network Type | Description                                                                         | Use Case                       |
| ------------ | ----------------------------------------------------------------------------------- | ------------------------------ |
| **bridge**   | Default for standalone containers. Isolated, needs `-p` to expose ports.            | Local dev                      |
| **host**     | Container shares host’s network stack. No `-p` needed, uses host IP directly.       | High perf, monitoring          |
| **none**     | No networking at all.                                                               | Security, testing              |
| **overlay**  | Multi-host networking (Swarm/Kubernetes).                                           | Distributed apps               |
| **macvlan**  | Assigns container its own MAC + IP from LAN. Appears as physical device on network. | Legacy apps, direct LAN access |

---

## 3. **Commands**

### **3.1. Network Management**

```bash
# List networks
docker network ls

# Inspect a network
docker network inspect mynet

# Create a bridge network
docker network create mynet

# Remove a network
docker network rm mynet
```

---

### **3.2. Connecting/Disconnecting Containers**

```bash
# Connect container to a network
docker network connect mynet web

# Disconnect container
docker network disconnect mynet web
```

---

## 4. **Docker Compose Examples**

### **4.1. Default (All in Same Network)**

```yaml
version: "3"
services:
  web:
    image: nginx
  db:
    image: mysql
```

👉 All services are in the same default network → can talk by service name.

---

### **4.2. Custom Networks**

```yaml
version: "3.9"
services:
  web:
    image: nginx
    networks:
      - frontend

  db:
    image: mysql
    networks:
      - backend

  api:
    image: myapp
    networks:
      - frontend
      - backend

networks:
  frontend:
  backend:
```

👉 Same as CLI example:

* `web ↔ api` works
* `api ↔ db` works
* `web ↔ db` blocked

---

### **4.3. No Outside World Access**

```yaml
version: "3.9"
services:
  secure:
    image: alpine
    command: ["sh", "-c", "ping google.com || echo NO INTERNET"]
    networks:
      - isolated

networks:
  isolated:
    driver: bridge
    internal: true   # 🔒 No internet access
```

👉 `internal: true` makes the network **internal only** = no internet.

---

## 5. **Practical Scenarios**

* **SSL Certificates (bind mounts + restricted networks)**
  Mount certs into container (`:ro`) and put service in a network only accessible to reverse proxy (NGINX/Traefik).

* **DB Isolation**
  Database in a private network, only API container has access.

* **Zero-Trust / Security**
  Containers with sensitive data use `--network none` or internal-only networks.

* **Microservices**
  Group microservices per logical layer (frontend, backend, db).

---

## ✅ TL;DR Rules

1. By default, containers **can talk to the internet**.
2. By default, containers **cannot talk to each other** unless on same user-defined network.
3. Use **internal networks** to block internet.
4. Use **multiple networks** to control which containers see which.
5. Use `--network none` for **total isolation**.

---
---

# 🔹 1. What is `n8n-docker_n8n_network`?

When you create a project with **Docker Compose**, it automatically creates a network for that project.

👉 The default naming convention is:

```
<project-name>_<network-name>
```

* **`project-name`** = folder name where your `docker-compose.yml` is, unless you set `-p <project>` when running `docker compose up`.
* **`network-name`** = the name you declare under `networks:` in your Compose file.

In your case:

```yaml
networks:
  n8n_network:
    driver: bridge
```

* The `network-name` is **`n8n_network`**.
* The Compose project name (looks like) is **`n8n-docker`**.
* So Docker auto-creates a network called:
  **`n8n-docker_n8n_network`**

✅ That’s why in Traefik config you wrote:

```yaml
--providers.docker.network=n8n-docker_n8n_network
```

This tells Traefik:

> “When I discover containers, use the `n8n-docker_n8n_network` network to talk to them.”

Otherwise, if the container is attached to multiple networks, Traefik wouldn’t know which one to use.

---

# 🔹 2. What does `--providers.docker.exposedbydefault=false` mean?

By default, Traefik **would route to all running containers** it sees via Docker.
That’s insecure in production — you don’t want *every container* (DBs, workers, test containers) being exposed publicly.

So you’ve told Traefik:

```yaml
--providers.docker.exposedbydefault=false
```

👉 Meaning: **Do not expose anything by default.**
Only expose containers that explicitly say:

```yaml
labels:
  - "traefik.enable=true"
```

---

# 🔹 3. How do you “label” containers to be exposed?

In Docker Compose, you add **labels** under each service. Example from your `n8n` service:

```yaml
labels:
  - "traefik.enable=true"
  - "traefik.http.routers.n8n.rule=Host(`vspace.store`)"
  - "traefik.http.services.n8n.loadbalancer.server.port=5678"
```

👉 Here’s what’s happening:

* `traefik.enable=true` → This container should be visible to Traefik.
* `traefik.http.routers.*` → Defines **routers** (match rules like `Host`, `PathPrefix`) that decide *when* traffic goes to this container.
* `traefik.http.services.*` → Defines **services** (the actual container + its internal port).
* Middlewares (rate-limit, strip-prefix, headers, IP whitelist) are attached as well.

Without the `traefik.enable=true`, Traefik would **ignore this container completely**.

---

# 🔹 4. How does Traefik “see” these labels?

This is where **the Docker socket** comes in:

```yaml
- /var/run/docker.sock:/var/run/docker.sock:ro
```

That’s the secret. 👀

* The Docker socket (`/var/run/docker.sock`) is the Unix socket that the Docker daemon listens on.
* By mounting it **into Traefik**, Traefik can “ask Docker”:

  * Which containers are running?
  * What networks are they connected to?
  * What labels are defined?

So Traefik listens for Docker events (container start/stop) and **dynamically reconfigures** itself.
That’s how Traefik knows about the labels you wrote inside Compose, even though Traefik is a separate container.

---

# 🔹 5. Who handles the labels?

* Labels are not “executed” by Docker itself.
* They are **metadata** attached to a container.
* Any system that integrates with Docker (like Traefik, Portainer, Caddy, Swarm, Kubernetes) can read those labels and apply its own logic.

So:

* **Docker** stores and exposes them.
* **Traefik** reads them (via socket).
* **You** define them in `docker-compose.yml`.

---

# 🔹 6. Example Workflow in Your Setup

1. You `docker compose up`.

   * Docker creates containers (`traefik`, `n8n`, `python-runner`).
   * Containers are attached to network `n8n-docker_n8n_network`.

2. Traefik queries Docker API via `/var/run/docker.sock`.

   * Sees container `n8n` has `traefik.enable=true`.
   * Reads router rule `Host(vspace.store)`.
   * Maps that hostname → `n8n:5678` inside the network.

3. A user visits `https://vspace.store`.

   * Request hits Traefik (ports `80`/`443`).
   * Traefik matches the router rule.
   * Traefik forwards the traffic **inside Docker** to `n8n:5678` (not exposed to host directly).

4. Same with `python-runner`, except you’ve also added:

   ```yaml
   - "traefik.http.middlewares.python-runner-ipwhitelist.ipwhitelist.sourcerange=172.18.0.0/16"
   ```

   That restricts access so only internal containers in that subnet (your `n8n` etc.) can call it.

---

# 🔹 7. Visual Summary

```
        [ World / Clients ]
                │
        ┌───────▼─────────┐
        │    Traefik      │   <-- listens on 80/443 (ports:)
        └───┬─────────────┘
            │
   Reads labels via Docker socket
            │
  ┌─────────▼──────────┐
  │ n8n container      │  <-- not exposed publicly
  │ Labels define:      │
  │   traefik.enable    │
  │   Host(vspace.store)│
  │   port=5678         │
  └─────────────────────┘
            │
  ┌─────────▼──────────┐
  │ python-runner       │ <-- only accessible via Traefik (not host)
  │ Labels + whitelist  │
  │ port=8000           │
  └─────────────────────┘
```

---

✅ So the short answers to your exact questions:

* **What is `n8n-docker_n8n_network`?**
  → The auto-generated Docker network name (projectName\_networkName).
* **What does `exposedbydefault=false` mean?**
  → Containers won’t be routed unless they have `traefik.enable=true`.
* **How to label containers?**
  → Using `labels:` in Compose under the service.
* **How does Traefik see them?**
  → Via Docker socket (`/var/run/docker.sock`), which exposes metadata including labels.

---

### 🔑 Key point:

- A **Dockerfile** does **not** define a network name.  
    It only defines how to build an image (base image, packages, environment, ports exposed, entrypoint, etc.).
    
- Networks are defined at **runtime** (in `docker-compose.yml` or via `docker network create …`), not in the `Dockerfile`.
    

---

### Why you’re seeing `n8n_network`

If your `docker-compose.yml` contains something like:

```yaml
networks:
  n8n_network:
    driver: bridge
```

and then under your service:

```yaml
services:
  n8n:
    networks:
      - n8n_network
```

👉 Then the actual Docker network created will be called **`<project-name>_n8n_network`**.  
By default, `docker-compose` prefixes everything (networks, volumes, containers) with the **project name**.  
The project name is usually the **folder name** where `docker-compose.yml` is located (unless you override it with `-p myproject`).

So, if your folder is `n8n`, the real network will be:

```
n8n_n8n_network
```

not just `n8n_network`.

---

### ✅ Ways to make it exactly `n8n_network`

If you want the network to appear **exactly** as `n8n_network` (no prefix):

```yaml
networks:
  n8n_network:
    name: n8n_network   # 👈 Explicit name
    driver: bridge
```

Now Docker will create a network named **`n8n_network`** instead of prefixing it.

---
---

### 🔎 Let’s unpack what’s happening

#### 1. `traefik.enable=true`

This label tells Traefik:
👉 “I am a service you should manage if my rules match.”

So yes, you’ve made the service *discoverable* to Traefik. But discovery alone doesn’t mean it’s **reachable from the outside world**.

---

#### 2. Why your API is not reachable at `https://vspace.store/api/python`

* In your `python-runner` service you used:

```yaml
expose:
  - "8000"  # Make it accessible only inside the Docker network
```

⚡ **`expose`** does not publish the port to the host machine.
It only makes the port available **to other containers in the same Docker network** (like Traefik).

So your FastAPI service on `8000` is **visible to Traefik** but **not directly exposed to the outside**.
That’s why if you curl `localhost:8000` on the host, nothing works.
But Traefik *can* see it inside the `n8n_network`.

---

#### 3. Why Traefik doesn’t serve it to the outside

Now, even though Traefik sees your service (`traefik.enable=true`, router/service labels), you added this middleware:

```yaml
- "traefik.http.middlewares.python-runner-ipwhitelist.ipwhitelist.sourcerange=172.18.0.0/16"
```

That tells Traefik:
👉 “Only allow requests coming from IPs in the Docker bridge network range (172.18.x.x).”

* Outside requests (e.g., you hitting `https://vspace.store/api/python`) come from your browser’s public IP, not from the Docker subnet.
* Traefik blocks them by design.

So, you ended up with exactly what you observed:
✅ `python-runner` is only accessible **from other containers inside the network** (like `n8n`).
❌ Not accessible from the public internet.

---

### 🎯 Why this is actually good

This setup enforces a **“service-to-service only”** API:

* Your FastAPI container (`python-runner`) is **hidden** from the world.
* Only `n8n` (or any other container on `n8n_network`) can call it at `http://python-runner:8000/…`.
* Traefik blocks external requests due to the **IP whitelist**.

This is a **best practice** when you don’t want an internal API to be public (e.g., worker services, private microservices).

---

### 📝 Quick Summary

| Setting                  | Meaning                                                                                                                                |
| ------------------------ | -------------------------------------------------------------------------------------------------------------------------------------- |
| `expose`                 | Only internal to Docker network (Traefik sees it, host doesn’t)                                                                        |
| `ports`                  | Publishes to host (outside world can reach it directly)                                                                                |
| `traefik.enable=true`    | Lets Traefik discover the container                                                                                                    |
| `ipwhitelist middleware` | Restricts who can talk to the service                                                                                                  |
| Your case                | Traefik discovers the service, but external traffic is rejected because it’s not from `172.18.0.0/16`. Perfect for internal-only APIs. |

---

👉 So the reason you cannot access `vspace.store/api/python` publicly is because you explicitly told Traefik **“only allow Docker subnet clients”**.
That’s why `n8n` can call it, but your browser cannot.

---
