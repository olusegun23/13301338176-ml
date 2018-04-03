A Docker image is like a virtual machine image.

A running instance of a Docker image is known as a Docker container. 

You could think of a Docker container as a virtual machine instance with almost no performance overhead.

## Install Docker Runtime

The Docker runtime is like a virtual machine management system, say, VirtualBox or VMware.

### Installers

1. For Mac: https://download.docker.com/mac/stable/Docker.dmg
1. For Windows: https://download.docker.com/win/stable/Docker%20for%20Windows%20Installer.exe
1. For Linux: https://docs.docker.com/engine/installation/

### Test if it's properly installed

Open 'Terminal'(Mac) or 'Commandline'(Windows), type the command `docker version`.  If you see both client and server versions like the following, you are ready to go.

``` text
$ docker version
Client:
 Version:      17.09.0-ce-rc1
 API version:  1.31
 Go version:   go1.8.3
 Git commit:   ae21824
 Built:        Wed Sep  6 22:25:36 2017
 OS/Arch:      darwin/amd64

Server:
 Version:      17.09.0-ce-rc1
 API version:  1.32 (minimum version 1.12)
 Go version:   go1.8.3
 Git commit:   ae21824
 Built:        Wed Sep  6 22:31:19 2017
 OS/Arch:      linux/amd64
 Experimental: true
```

## Usage

### To download and run an image

```bash
docker run paddlepaddle/book
```

where `paddlepaddle/book` is the image id in Dockerhub. Here we omit tag name, docker will use `latest` by default.

If you see messages of the following form

```
Cannot connect to the Docker daemon at unix:///var/run/docker.sock. Is the docker daemon running?
```

It means you need to run Docker so the daemon is on.

### To deploy, run an image and interact with it via command line

```bash
docker run -t paddlepaddle/book
```

where `-t` means keeping the instance in the foreground and interact with it like you logged in a remote machine.

### To deploy, run, interact and also mount a local folder into it

```bash
docker run -t -v /foo:/bar paddlepaddle/book
```

where `-v /foo:/bar` means mounting your local machine's folder '/foo' to container's '/bar' path

### To deploy, run, interact, mount, and forward a host port to container's

```bash
docker run -t -v /foo:/bar -p 80:8080  paddlepaddle/book
```

where `-p 80:8080` means any request to host's 80 port will be forwarded to container's 8080 port.

### To list all containers running

```bash
docker ps
```

if you need to list containers in 'stopped' state, use `docker ps -a`.

### To stop a container

```bash
docker stop 591a09a8a5be
```

where `591a09a8a5be` is the container id which you can find from `docker ps`.

if `docker stop` is not powerful enough, try `docker kill`

### To restart a container

```bash
docker restart 591a09a8a5be
```

### To re enter a container
1. first make sure the container is running. use
```bash
docker ps -a
```
and check `STATUS`.

2. attach it
```bash
docker attach $container_id
```

where `591a09a8a5be` is the container id.

## Additonal Documentation

https://docs.docker.com/