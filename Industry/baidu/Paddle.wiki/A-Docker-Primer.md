# A Docker Primer

> "[Docker](http://www.docker.io/) is a great tool for creating and managing **lightweight** **isolated** application environments, a.k.a. containers. Docker is a wonderful layer on top of Linux containers (LXC). If you’re not familiar with Linux containers, the easiest way to think of them is like extremely lightweight VMs – they allow Linux operating systems to host other copies of Linux, safely sharing access to resources without the overhead of something like Xen or VirtualBox."
>
> -- <cite> https://ochronus.com/docker-primer-django/ </cite>

## Basic Concepts

Docker is simple as long as we understand a few basic concepts:

- *image*: A Docker image is a pack of software. It could contain one or more programs and all their dependencies. For example, the PaddlePaddle's Docker image includes pre-built PaddlePaddle and Python and many Python packages. We can run a Docker image directly, other than installing all these software. We can type
  ```bash
  docker images
  ```
  to list all images in the system. We can also run

  ```bash
  docker pull paddlepaddle/paddle:0.10.0rc2
  ```

  to download a Docker image, paddlepaddle/paddle in this example,
  from Dockerhub.com.

- *container*: considering a Docker image a program, a container is a
  "process" that runs the image. Indeed, a container is exactly an
  operating system process, but with a virtualized filesystem, network
  port space, and other virtualized environment. We can type
  ```bash
  docker run paddlepaddle/paddle:0.10.0rc2
  ```

  to start a container to run a Docker image, paddlepaddle/paddle in this example.

- By default docker container have an isolated file system namespace,
  we can not see the files in the host file system. By using *volume*,
  mounted files in host will be visible inside docker container.
  Following command will mount current dirctory into /data inside
  docker container, run docker container from debian image with
  command :code:`ls /data`.
  ```bash
  docker run --rm -v $(pwd):/data debian ls /data
  ```

## Lightweight

### Almost No Overhead
In fact, it is just a native process (if running on a linux host system):
```bash
docker run -d -p 8000:80 nginx # visit http://localhost:8000/ you will see nginx welcome page
CONTAINER_ID=$(docker ps -lq)
docker top $CONTAINER_ID # display the running processes of a container
```
We get:
```text
UID    PID    PPID    C    STIME    TTY    TIME      CMD
root   16205  16188   0    15:45    ?      00:00:00  nginx: master process nginx -g daemon off;
syslog 16243  16205   0    15:45    ?      00:00:00  nginx: worker process
```
On your own linux system:
```bash
ps aux|grep 16205|grep -v grep; ps aux|grep 16243|grep -v grep
```
```text
root     16205  0.0  0.0  31876  5300 ? Ss 15:45 0:00 nginx: master process nginx -g daemon off;
syslog   16243  0.0  0.0  32264  2844 ? S  15:45 0:00 nginx: worker process
```

More details: https://www.quora.com/What-is-the-overhead-to-have-docker-containers-Running-on-top-of-Vmware-and-hyper-v-hypervisors-hosts

### Very Small Image Size

A "OS" image can be as small as 3.99Mb
```bash
docker pull alpine
docker images|grep alpine
```

```text
alpine    latest    baa5d63471ea    5 weeks ago    3.99 MB
```

And it's fully functional:
```bash
docker run -it alpine /bin/sh  
```

You can even just run a statically built binary on an empty image (called [scratch](https://hub.docker.com/_/scratch/)): http://blog.xebia.com/create-the-smallest-possible-docker-container/

## Isolated

Isolation is achieved using namespace and cgroup.

### Cgroup: Resource Metering and Limiting

- CPU
- Memory
- Block I/O
- Network
- Device node access control (`/dev/*`)

### Namespace

- Pid: process ID tree outside of container not visible
- Net: private network stack: network interface, routing tables, iptables rules
- Mnt: process can have their own root fs
- IPC: own IPC message queues, shared memory
- user: private UID, GID

More info: https://www.slideshare.net/jpetazzo/anatomy-of-a-container-namespaces-cgroups-some-filesystem-magic-linuxcon, https://www.slideshare.net/jpetazzo/introduction-docker-linux-containers-lxc

## nvidia-docker

Run PaddlePaddle with GPU support requires:
- nvidia driver
- CUDA
- CUDNN

CUDA and CUDNN is packaged inside docker image ([see here](https://github.com/PaddlePaddle/Paddle/blob/develop/Dockerfile#L3)). They don’t have to be installed inside host machine.

A pain point is nvidia driver contains the kernel object (ko) and the shared library (so). The ko is already loaded into host kernel. Userspace binary inside the container will need the so file, and the so file needs to match the ko file’s version. It’s no possible to know host machine’s nvidia driver version when packaging docker image.

[nvidia-docker](https://github.com/NVIDIA/nvidia-docker):
- automatically mounts the correct so file into the container.
- automatically expose GPU devices (/dev/nvidia*) to the container.