In Docker, each layer represents a set of filesystem changes. When you run a command in a Dockerfile that modifies the filesystem (like COPY, ADD, RUN), Docker captures the changes made by that command as a "layer". Each layer is built on top of the previous one, forming a stack of layers that make up your Docker image.

### Understanding Docker Layers

Here’s a breakdown of how each type of command in a Dockerfile contributes to layer formation:

1. **FROM**: This starts the base of your image and does not build on top of another layer. It establishes the initial filesystem state from the specified base image.

2. **RUN**: Executes a command and captures the resulting filesystem state as a new layer. For example, installing software packages or modifying configuration files.

3. **COPY** and **ADD**: These commands copy files from the host machine into the Docker image and create a new layer with these additions. ADD can also handle URLs and tar files, automatically unpacking tar files into the image.

4. **CMD** and **ENTRYPOINT**: These do not typically create a new layer as they set default commands and parameters that will be executed when the Docker container starts. They only modify metadata of the image.

5. **ENV**, **ARG**, **EXPOSE**, **VOLUME**, **USER**, and other similar commands: These modify the environment configuration or metadata for the image but do not typically add a new filesystem layer. However, they do create a new layer in the image's metadata.

6. **WORKDIR**: Sets the working directory for any RUN, CMD, ENTRYPOINT, COPY, and ADD instructions that follow in the Dockerfile. It creates a layer if the specified directory does not already exist.

### Visualizing Layers

To visualize what each layer looks like, you can think of each layer as a transparent sheet. Each sheet (layer) has changes made to the filesystem written on it. When you stack these sheets one on top of the other, you get the final filesystem of your Docker image.

### Example

Consider a simple Dockerfile:

```Dockerfile
FROM ubuntu:20.04
RUN apt-get update && apt-get install -y curl
COPY . /app
WORKDIR /app
RUN echo "Hello, Docker!" > welcome.txt
CMD ["cat", "welcome.txt"]
```

Here's how the layers would look:

- **Layer 1**: The Ubuntu 20.04 filesystem.
- **Layer 2**: Updates to the package list and the installation of curl.
- **Layer 3**: Files from the current directory on the host copied to `/app` in the image.
- **Layer 4**: Creation of `/app` if it doesn't exist and setting it as the working directory.
- **Layer 5**: A new file `welcome.txt` containing "Hello, Docker!".
- **Metadata Layer**: The default command to run `cat welcome.txt`.

### Tools for Visualization

You can use tools like Dive or Portainer to visually inspect these layers in a more interactive and graphical manner. These tools provide a UI to explore each layer, see the changes, and understand the impact on the image size.

By using these tools or commands like `docker history`, you can get a detailed view of what each layer contains and how they contribute to the final Docker image.