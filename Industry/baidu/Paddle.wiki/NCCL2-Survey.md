## NCCL2 Survey



- NCCL 1.0 support one machine multi devices, devices use PCIe、NVlink、GPU Direct P2P to communicate with each other.
- NCCL 2.0 support multi machines，machines use Sockets(Ethernet) or InfiniBand with GPU Direct RDMA to communicate with each other。


## Feature of NCCL2
* Multi-gpu and multi-node communication collectives such as:
  - all-gather
  - all-reduce
  - broadcast
  - reduce
  - reduce-scatter
* Automatic topology detection to determine optimal communication path
Optimized to achieve high bandwidth over PCIe and NVLink high-speed interconnect
* Support multi-threaded and multiprocess applications
Multiple ring formations for high bus utilization
* Support for InfiniBand verbs, RoCE and IP Socket internode communication


## Advantage
 - Delivers over 90% multi-node scaling efficiency using up to eight GPU-accelerated servers
 - Performs automatic topology detection to determine optimal communication path
 - Optimized to achieve high bandwidth over PCIe and NVLink high-speed interconnect
 - Support for RDMA over Converged Ethernet (RoCE) network protocol for multi-node scaling

## Problems
 - NCCL 2.0 is not open sourced, it provides a deb install file, [install guide](http://docs.nvidia.com/deeplearning/sdk/nccl-install-guide/index.html). It contains a header file and two lib:

	```shell
	/usr/lib/x86_64-linux-gnu/libnccl_static.a
	/usr/lib/x86_64-linux-gnu/libnccl.so -> libnccl.so.2
	/usr/include/nccl.h
	```
 - use CUDA8 or above
 - only support Ubuntu 14.04/Ubuntu 16.04/agnostic Operating system.
 - does not support auto scaling

 
## Usage
NCCL follows the MPI collectives API fairly closely. Before any collectives can be called, a `communicator` object must be initialized on each GPU. 

On a single-process machine, all GPUs can be conveniently initialized using `ncclCommInitAll`. For multi-process applications (e.g., with MPI), ncclCommInitRank must be called for each GPU. Internally `ncclCommInitRank` invokes a synchronization among all GPUs, so these calls must be invoked in different host threads (or processes) for each GPU. A brief single-process example follows, for an MPI example see test/mpi/mpi_test.cu. For details about the API see nccl.h.

```cpp
#include <nccl.h>

typedef struct {
  double* sendBuff;
  double* recvBuff;
  int size;
  cudaStream_t stream;
} PerThreadData;

int main(int argc, char* argv[])
{
  int nGPUs;
  cudaGetDeviceCount(&nGPUs);
  ncclComm_t* comms = (ncclComm_t*)malloc(sizeof(ncclComm_t)*nGPUs);
  ncclCommInitAll(comms, nGPUs); // initialize communicator
                                // One communicator per process

  PerThreadData* data;

  ... // Allocate data and issue work to each GPU's
      // perDevStream to populate the sendBuffs.

  for(int i=0; i<nGPUs; ++i) {
    cudaSetDevice(i); // Correct device must be set
                      // prior to each collective call.
    ncclAllReduce(data[i].sendBuff, data[i].recvBuff, size,
        ncclDouble, ncclSum, comms[i], data[i].stream);
  }

  ... // Issue work into data[*].stream to consume buffers, etc.
}
```

## Platform that use NCCL

### Pytorch

Pytorch use nccl 1, wrap it as Python API

https://github.com/pytorch/pytorch/blob/master/torch/csrc/cuda/Module.cpp#L420
https://github.com/pytorch/pytorch/blob/master/torch/cuda/nccl.py

for distribute training, pytorch write a module named [`THD`](https://github.com/pytorch/pytorch/issues/241)

### Tensorflow
Tensorflow use NCCL 1 as contrib module, and also wrap it in Python API
https://www.tensorflow.org/api_docs/python/tf/contrib/nccl


### Uber horovod
horovod is based on Tensorflow and NCCL 2 to improve the performance of tensorflow distributed training.

NCCL 2 introduced the ability to run ring-allreduce across multiple machines, enabling us to take advantage of its many performance boosting optimizations.

### refs
* https://developer.nvidia.com/nccl
* https://devblogs.nvidia.com/parallelforall/fast-multi-gpu-collectives-nccl/
* http://docs.nvidia.com/deeplearning/sdk/nccl-install-guide/index.html
* https://eng.uber.com/horovod/
* https://github.com/uber/horovod/tree/master/examples
