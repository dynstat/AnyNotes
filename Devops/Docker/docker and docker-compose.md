# Docker Commands Reference

This document provides a comprehensive list of Docker commands related to building, running, managing volumes, services, networks, executing containers, tagging, pushing, logging in, pulling images, cleaning, and more. Additionally, it covers similar and advanced commands using `docker-compose`. Each command includes a brief explanation and practical usage examples.

## Table of Contents

1. [Docker CLI Commands](#docker-cli-commands)
    - [Image Management](#image-management)
        - [`docker build`](#docker-build)
        - [`docker pull`](#docker-pull)
        - [`docker push`](#docker-push)
        - [`docker tag`](#docker-tag)
        - [`docker rmi`](#docker-rmi)
        - [`docker image prune`](#docker-image-prune)
    - [Container Management](#container-management)
        - [`docker run`](#docker-run)
        - [`docker exec`](#docker-exec)
        - [`docker start`](#docker-start)
        - [`docker stop`](#docker-stop)
        - [`docker restart`](#docker-restart)
        - [`docker rm`](#docker-rm)
        - [`docker container prune`](#docker-container-prune)
    - [Volume Management](#volume-management)
        - [`docker volume create`](#docker-volume-create)
        - [`docker volume ls`](#docker-volume-ls)
        - [`docker volume inspect`](#docker-volume-inspect)
        - [`docker volume rm`](#docker-volume-rm)
        - [`docker volume prune`](#docker-volume-prune)
    - [Network Management](#network-management)
        - [`docker network create`](#docker-network-create)
        - [`docker network ls`](#docker-network-ls)
        - [`docker network inspect`](#docker-network-inspect)
        - [`docker network rm`](#docker-network-rm)
        - [`docker network prune`](#docker-network-prune)
    - [Service Management](#service-management)
        - [`docker service create`](#docker-service-create)
        - [`docker service ls`](#docker-service-ls)
        - [`docker service inspect`](#docker-service-inspect)
        - [`docker service update`](#docker-service-update)
        - [`docker service rm`](#docker-service-rm)
    - [Authentication](#authentication)
        - [`docker login`](#docker-login)
        - [`docker logout`](#docker-logout)
    - [System Cleanup](#system-cleanup)
        - [`docker system prune`](#docker-system-prune)
2. [Docker Compose Commands](#docker-compose-commands)
    - [Basic Commands](#basic-commands)
        - [`docker-compose up`](#docker-compose-up)
        - [`docker-compose down`](#docker-compose-down)
        - [`docker-compose build`](#docker-compose-build)
        - [`docker-compose push`](#docker-compose-push)
        - [`docker-compose pull`](#docker-compose-pull)
    - [Advanced Commands](#advanced-commands)
        - [`docker-compose logs`](#docker-compose-logs)
        - [`docker-compose exec`](#docker-compose-exec)
        - [`docker-compose scale`](#docker-compose-scale)
        - [`docker-compose config`](#docker-compose-config)
        - [`docker-compose run`](#docker-compose-run)
        - [`docker-compose down with Cleanup`](#docker-compose-down-with-cleanup)
3. [Practical Usage Examples](#practical-usage-examples)
    - [Building and Running a Docker Container](#building-and-running-a-docker-container)
    - [Using Docker Compose to Manage Multi-Container Applications](#using-docker-compose-to-manage-multi-container-applications)
    - [Cleaning Up Docker Resources](#cleaning-up-docker-resources)

---

## Docker CLI Commands

### Image Management

#### `docker build`

Build an image from a Dockerfile.

**Syntax:**
```bash
docker build [OPTIONS] PATH | URL | -
```

**Example:**
```bash
docker build -t myapp:latest .
```

- `-t myapp:latest`: Tags the image with the name `myapp` and tag `latest`.
- `.`: Specifies the current directory as the build context.

#### `docker pull`

Pull an image or a repository from a registry.

**Syntax:**
```bash
docker pull [OPTIONS] NAME[:TAG|@DIGEST]
```

**Example:**
```bash
docker pull nginx:alpine
```

- `nginx:alpine`: Pulls the `alpine` tagged version of the `nginx` image.

#### `docker push`

Push an image or repository to a registry.

**Syntax:**
```bash
docker push [OPTIONS] NAME[:TAG]
```

**Example:**
```bash
docker push myregistry.com/myapp:latest
```

- `myregistry.com/myapp:latest`: Pushes the `latest` tag of `myapp` to `myregistry.com`.

#### `docker tag`

Create a tag `TARGET_IMAGE` that refers to `SOURCE_IMAGE`.

**Syntax:**
```bash
docker tag SOURCE_IMAGE[:TAG] TARGET_IMAGE[:TAG]
```

**Example:**
```bash
docker tag myapp:latest myregistry.com/myapp:v1.0
```

- Tags the existing `myapp:latest` image as `myregistry.com/myapp:v1.0`.

#### `docker rmi`

Remove one or more images.

**Syntax:**
```bash
docker rmi [OPTIONS] IMAGE [IMAGE...]
```

**Example:**
```bash
docker rmi myapp:latest
```

- Removes the `myapp:latest` image from the local registry.

**Options:**
- `-f`, `--force`: Force removal of the image.

#### `docker image prune`

Remove unused images to free up space.

**Syntax:**
```bash
docker image prune [OPTIONS]
```

**Example:**
```bash
docker image prune -a
```

- `-a`, `--all`: Remove all unused images, not just dangling ones.

---

### Container Management

#### `docker run`

Run a command in a new container.

**Syntax:**
```bash
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```

**Example:**
```bash
docker run -d -p 80:80 --name webserver nginx:alpine
```

- `-d`: Runs the container in detached mode.
- `-p 80:80`: Maps host port 80 to container port 80.
- `--name webserver`: Names the container `webserver`.
- `nginx:alpine`: Specifies the image to use.

#### `docker exec`

Run a command in a running container.

**Syntax:**
```bash
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```

**Example:**
```bash
docker exec -it webserver /bin/sh
```

- `-it`: Opens an interactive terminal.
- `webserver`: Target container name.
- `/bin/sh`: Command to execute inside the container.

#### `docker start`

Start one or more stopped containers.

**Syntax:**
```bash
docker start [OPTIONS] CONTAINER [CONTAINER...]
```

**Example:**
```bash
docker start webserver
```

- Starts the `webserver` container.

#### `docker stop`

Stop one or more running containers.

**Syntax:**
```bash
docker stop [OPTIONS] CONTAINER [CONTAINER...]
```

**Example:**
```bash
docker stop webserver
```

- Stops the `webserver` container.

#### `docker restart`

Restart one or more containers.

**Syntax:**
```bash
docker restart [OPTIONS] CONTAINER [CONTAINER...]
```

**Example:**
```bash
docker restart webserver
```

- Restarts the `webserver` container.

#### `docker rm`

Remove one or more containers.

**Syntax:**
```bash
docker rm [OPTIONS] CONTAINER [CONTAINER...]
```

**Example:**
```bash
docker rm webserver
```

- Removes the `webserver` container.

**Options:**
- `-f`, `--force`: Force the removal of a running container (uses SIGKILL).

#### `docker container prune`

Remove all stopped containers.

**Syntax:**
```bash
docker container prune [OPTIONS]
```

**Example:**
```bash
docker container prune -f
```

- `-f`, `--force`: Do not prompt for confirmation.

---

### Volume Management

#### `docker volume create`

Create a new volume.

**Syntax:**
```bash
docker volume create [OPTIONS] [VOLUME]
```

**Example:**
```bash
docker volume create mydata
```

- Creates a volume named `mydata`.

#### `docker volume ls`

List volumes.

**Syntax:**
```bash
docker volume ls [OPTIONS]
```

**Example:**
```bash
docker volume ls
```

- Lists all Docker volumes.

#### `docker volume inspect`

Display detailed information on one or more volumes.

**Syntax:**
```bash
docker volume inspect [OPTIONS] VOLUME [VOLUME...]
```

**Example:**
```bash
docker volume inspect mydata
```

- Shows details of the `mydata` volume.

#### `docker volume rm`

Remove one or more volumes.

**Syntax:**
```bash
docker volume rm [OPTIONS] VOLUME [VOLUME...]
```

**Example:**
```bash
docker volume rm mydata
```

- Deletes the `mydata` volume.

#### `docker volume prune`

Remove all unused volumes.

**Syntax:**
```bash
docker volume prune [OPTIONS]
```

**Example:**
```bash
docker volume prune -f
```

- `-f`, `--force`: Do not prompt for confirmation.

---

### Network Management

#### `docker network create`

Create a new network.

**Syntax:**
```bash
docker network create [OPTIONS] NETWORK
```

**Example:**
```bash
docker network create mynetwork
```

- Creates a network named `mynetwork`.

#### `docker network ls`

List networks.

**Syntax:**
```bash
docker network ls [OPTIONS]
```

**Example:**
```bash
docker network ls
```

- Lists all Docker networks.

#### `docker network inspect`

Display detailed information on one or more networks.

**Syntax:**
```bash
docker network inspect [OPTIONS] NETWORK [NETWORK...]
```

**Example:**
```bash
docker network inspect mynetwork
```

- Shows details of the `mynetwork` network.

#### `docker network rm`

Remove one or more networks.

**Syntax:**
```bash
docker network rm NETWORK [NETWORK...]
```

**Example:**
```bash
docker network rm mynetwork
```

- Deletes the `mynetwork` network.

#### `docker network prune`

Remove all unused networks.

**Syntax:**
```bash
docker network prune [OPTIONS]
```

**Example:**
```bash
docker network prune -f
```

- `-f`, `--force`: Do not prompt for confirmation.

---

### Service Management

*(Requires Docker Swarm mode)*

#### `docker service create`

Create a new service.

**Syntax:**
```bash
docker service create [OPTIONS] IMAGE [COMMAND] [ARG...]
```

**Example:**
```bash
docker service create --name myservice --replicas 3 nginx:alpine
```

- `--name myservice`: Names the service `myservice`.
- `--replicas 3`: Specifies 3 replicas of the service.
- `nginx:alpine`: Uses the `nginx:alpine` image.

#### `docker service ls`

List services.

**Syntax:**
```bash
docker service ls [OPTIONS]
```

**Example:**
```bash
docker service ls
```

- Lists all services in the swarm.

#### `docker service inspect`

Display detailed information about one or more services.

**Syntax:**
```bash
docker service inspect [OPTIONS] SERVICE [SERVICE...]
```

**Example:**
```bash
docker service inspect myservice
```

- Shows details of `myservice`.

#### `docker service update`

Update a service.

**Syntax:**
```bash
docker service update [OPTIONS] SERVICE
```

**Example:**
```bash
docker service update --replicas 5 myservice
```

- Updates `myservice` to have 5 replicas.

#### `docker service rm`

Remove one or more services.

**Syntax:**
```bash
docker service rm SERVICE [SERVICE...]
```

**Example:**
```bash
docker service rm myservice
```

- Deletes the `myservice` service.

---

### Authentication

#### `docker login`

Log in to a Docker registry.

**Syntax:**
```bash
docker login [OPTIONS] [SERVER]
```

**Example:**
```bash
docker login myregistry.com
```

- Prompts for username and password to log in to `myregistry.com`.

#### `docker logout`

Log out from a Docker registry.

**Syntax:**
```bash
docker logout [SERVER]
```

**Example:**
```bash
docker logout myregistry.com
```

- Logs out from `myregistry.com`.

---

### System Cleanup

#### `docker system prune`

Remove unused data including containers, networks, images, and optionally, volumes.

**Syntax:**
```bash
docker system prune [OPTIONS]
```

**Example:**
```bash
docker system prune -a --volumes
```

- `-a`, `--all`: Remove all unused images not just dangling ones.
- `--volumes`: Also remove all unused volumes.

**Description:**
This command helps in freeing up disk space by removing:
- Stopped containers
- Unused networks
- Dangling images
- Build cache
- (With `--volumes`) Unused volumes

**Warning:**
Use this command with caution as it will remove data that is not currently in use by any containers.

---

## Docker Compose Commands

`docker-compose` is a tool for defining and running multi-container Docker applications. It uses a YAML file to configure the application's services.

### Basic Commands

#### `docker-compose up`

Builds, (re)creates, starts, and attaches to containers for a service.

**Syntax:**
```bash
docker-compose up [OPTIONS] [SERVICE...]
```

**Example:**
```bash
docker-compose up -d
```

- `-d`: Runs containers in detached mode.

#### `docker-compose down`

Stops and removes containers, networks, images, and volumes created by `docker-compose up`.

**Syntax:**
```bash
docker-compose down [OPTIONS]
```

**Example:**
```bash
docker-compose down
```

- Stops and removes all services defined in the `docker-compose.yml`.

#### `docker-compose build`

Build or rebuild services.

**Syntax:**
```bash
docker-compose build [OPTIONS] [SERVICE...]
```

**Example:**
```bash
docker-compose build web
```

- Builds the `web` service.

#### `docker-compose push`

Push service images.

**Syntax:**
```bash
docker-compose push [OPTIONS] [SERVICE...]
```

**Example:**
```bash
docker-compose push
```

- Pushes all service images to the registry.

#### `docker-compose pull`

Pull service images.

**Syntax:**
```bash
docker-compose pull [OPTIONS] [SERVICE...]
```

**Example:**
```bash
docker-compose pull
```

- Pulls all service images from the registry.

### Advanced Commands

#### `docker-compose logs`

View output from containers.

**Syntax:**
```bash
docker-compose logs [OPTIONS] [SERVICE...]
```

**Example:**
```bash
docker-compose logs -f web
```

- `-f`: Follows log output.
- `web`: Shows logs for the `web` service.

#### `docker-compose exec`

Execute a command in a running service.

**Syntax:**
```bash
docker-compose exec [OPTIONS] SERVICE COMMAND [ARGS...]
```

**Example:**
```bash
docker-compose exec web /bin/bash
```

- Opens a bash shell in the `web` service container.

#### `docker-compose scale`

Sets the number of containers to run for a service. *(Deprecated in favor of `docker-compose up --scale`)*

**Syntax:**
```bash
docker-compose scale SERVICE=NUM
```

**Example:**
```bash
docker-compose scale web=3
```

- Runs 3 instances of the `web` service.

*Note: Use `docker-compose up --scale web=3 -d` instead.*

#### `docker-compose config`

Validate and view the Compose file.

**Syntax:**
```bash
docker-compose config [OPTIONS]
```

**Example:**
```bash
docker-compose config
```

- Merges and validates the `docker-compose.yml` configuration.

#### `docker-compose run`

Run a one-off command on a service.

**Syntax:**
```bash
docker-compose run [OPTIONS] SERVICE [COMMAND] [ARGS...]
```

**Example:**
```bash
docker-compose run web python manage.py migrate
```

- Runs Django migrations in the `web` service container.

#### `docker-compose down with Cleanup`

Stops containers and removes containers, networks, images, and optionally, volumes.

**Syntax:**
```bash
docker-compose down [OPTIONS]
```

**Example:**
```bash
docker-compose down --rmi all --volumes
```

- `--rmi all`: Removes all images used by any service.
- `--volumes`: Removes named volumes declared in the `volumes` section of the Compose file and anonymous volumes attached to containers.

---

## Practical Usage Examples

### Building and Running a Docker Container

1. **Build the Image:**
    ```bash
    docker build -t myapp:latest .
    ```
    - Builds an image named `myapp` with the tag `latest` using the current directory's `Dockerfile`.

2. **Run the Container:**
    ```bash
    docker run -d -p 8080:80 --name myrunningapp myapp:latest
    ```
    - Runs the `myapp:latest` image in detached mode.
    - Maps host port `8080` to container port `80`.
    - Names the container `myrunningapp`.

### Using Docker Compose to Manage Multi-Container Applications

1. **Define Services in `docker-compose.yml`:**
    ```yaml
    version: '3.8'

    services:
      web:
        build: .
        ports:
          - "8080:80"
        volumes:
          - .:/code
      db:
        image: postgres:13
        environment:
          POSTGRES_USER: user
          POSTGRES_PASSWORD: pass
    ```

2. **Start Services:**
    ```bash
    docker-compose up -d
    ```
    - Builds and starts the `web` and `db` services in detached mode.

3. **View Logs:**
    ```bash
    docker-compose logs -f web
    ```
    - Follows the logs of the `web` service.

4. **Execute a Command in the Web Service:**
    ```bash
    docker-compose exec web /bin/bash
    ```
    - Opens a bash shell in the `web` service container.

5. **Stop and Remove Services:**
    ```bash
    docker-compose down
    ```
    - Stops and removes all containers, networks, and default volumes.

### Cleaning Up Docker Resources

1. **Remove Unused Images:**
    ```bash
    docker image prune -a
    ```
    - Removes all unused images not referenced by any container.

2. **Remove Stopped Containers:**
    ```bash
    docker container prune -f
    ```
    - Forcefully removes all stopped containers without prompting for confirmation.

3. **Remove Unused Volumes:**
    ```bash
    docker volume prune -f
    ```
    - Removes all volumes not referenced by any container.

4. **Remove Unused Networks:**
    ```bash
    docker network prune -f
    ```
    - Deletes all unused Docker networks.

5. **Clean Up the Entire System:**
    ```bash
    docker system prune -a --volumes
    ```
    - Removes all stopped containers, all unused networks, all dangling images, and all unused volumes.

6. **Remove Specific Images:**
    ```bash
    docker rmi myapp:latest
    ```
    - Deletes the `myapp:latest` image from the local registry.

7. **Using Docker Compose to Remove Images and Volumes:**
    ```bash
    docker-compose down --rmi all --volumes
    ```
    - Stops containers and removes all related images and volumes defined in the `docker-compose.yml`.

**Note:** Always ensure that you do not need the resources you are about to remove, as these commands can delete data that may be difficult or impossible to recover.

---

