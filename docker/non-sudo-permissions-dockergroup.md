## Permissions to new non sudo user for docker

For permission denied error when trying to run Docker commands as a non-root user, add that user to the `docker` group:

```bash
sudo usermod -aG docker $USER
newgrp docker
```

Explanation:
1. The first command adds your current user to the `docker` group.
2. The second command activates the changes to groups without needing to log out and back in.

After running these commands, try the `docker ps` command again. It should work without permission errors.

Example with comments:
```bash
# Add the current user to the docker group
sudo usermod -aG docker $USER

# Activate the new group membership
newgrp docker

# Now try running docker ps again
docker ps
```

This works because Docker daemon binds to a Unix socket instead of a TCP port by default. This Unix socket is owned by the root user and other users can access it via the `docker` group. When we add a user to the `docker` group, they get the necessary permissions to use Docker commands without sudo.

Remember that adding a user to the `docker` group grants them root-equivalent permissions for Docker operations, so be cautious about which users you add to this group.


NOTE: If the issue persists after creating a new terminal, take a few additional steps.. :

1. First, let's verify that your user has been added to the docker group:

```bash
groups $USER
```

If you don't see 'docker' in the output, run this command again:

```bash
sudo usermod -aG docker $USER
```

2. Next, let's restart the Docker daemon:

```bash
sudo systemctl restart docker
```

3. Now, log out of your current session completely and log back in. This ensures that your user session fully recognizes the new group membership.

4. After logging back in, open a new terminal and try:

```bash
docker ps
```

If you're still encountering issues, let's check a few more things:

5. Verify Docker service status:

```bash
sudo systemctl status docker
```

This should show that Docker is active and running.

6. Check Docker socket permissions:

```bash
ls -l /var/run/docker.sock
```

The output should look something like this:
```
srw-rw---- 1 root docker 0 [date] /var/run/docker.sock
```

If the group is not 'docker', run:

```bash
# Change the ownership of the Docker socket file to root and the docker group
sudo chown root:docker /var/run/docker.sock

7. If none of the above works, you might need to reboot your system:

```bash
sudo reboot
```

After the reboot, try `docker ps` again. 