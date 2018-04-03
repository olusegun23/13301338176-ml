## Survey of CUDA Unified-Memory

### Defination
Unified Memory creates a pool of managed memory that is shared between the CPU and GPU, bridging the CPU-GPU divide. Managed memory is accessible to both the CPU and GPU using a single pointer. The key is that the system automatically migrates data allocated in Unified Memory between host and device so that it looks like CPU memory to code running on the CPU, and like GPU memory to code running on the GPU.

### Programming model
### CPU code
```cpp
void sortfile(FILE *fp, int N) {
	char *data;
	data = (char *)malloc(N);
	fread(data, 1, N, fp);
	qsort(data, N, 1, compare);
	use_data(data);
	free(data);
}
```

### GPU code with Unified Memory
```cpp
void sortfile(FILE *fp, int N) {
	char *data;
	cudaMallocManaged(&data, N);
	fread(data, 1, N, fp);
	qsort<<<...>>>(data,N,1,compare);
	cudaDeviceSynchronize();
	use_data(data);
	cudaFree(data);
}
```

## advantages
#### 1. Simpler Programming & Memory Model
 - Single pointer to data, accessible anywhere
 - Tight language integration
 - Greatly simplifies code porting

#### Performance Through Data Locality
  - Migrate data to accessing processor
  - Guarantee global coherency
  - Still allows cudaMemcpyAsync() hand tuning

### good usage
 Eliminate Deep Copies


### performance
#### speed
a little slower(1.0 ~ 1.1) then explicit copy because of redundant memory transfers

## Problems

#### comment from google tensorflow
1. speed is a problem

UVM is implemented through page-fault between CPU and GPU. It makes GPU program easier to be adopted, but not necessarily as fast as possible. Without more investigation, we are not sure this is suitable for high-performance machine learning. Note that the typical bandwidth across PCIE is often about two orders of magnitude slower than accessing the GPU dram itself.

If the model is indeed too large and cannot fit into GPU memory, it may make sense to load parts of the model in parallel, instead of relying the page faults in the kernels to page in the data.

CUDA transparently migrates dirty pages, which can be slow. Therefore, this allocator is intended `for convenience in functional tests only`.

## Frameworks that use CUDA Unified-Memory
1. tensorflow add a [`gpu_managed_allocator.h`](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/common_runtime/gpu/gpu_managed_allocator.h#L23) for test only.
2. other framework does not support Unified-Memory.


## conclusion
1. may have some conflict with buddy allocator. host memory is much bigger then device memory.
1. Unified-Memory see all GPU memory as one big memory.
1. Each data will both use CPU memory and GPU memory, if most of operators just need GPU memory, this is a huge waste of host memory.
1. If we want to support other device that with independent memory, such as FPGA, we still need to copy data between devices, so it's better to have a auto sync method for different kind of devices.

## 结论
1. 速度比显示拷贝要慢，因为会多做一些数据同步的事儿
1. 把所有GPU memory看成一个大的memory，不方便控制使用
1. 分配存储的时候会同时分配GPU存储和CPU存储
1. 未来如果我们要支持其他自带存储的设备，比如FPGA，依然需要一个内存同步测策略，Unified Memory搞不定。
1. tensorflow用Unified Memory来做测试框架，这个倒是可以参考

## Refs
- https://devblogs.nvidia.com/parallelforall/unified-memory-in-cuda-6/
- https://devblogs.nvidia.com/parallelforall/unified-memory-cuda-beginners/
- https://github.com/tensorflow/tensorflow/issues/3678
- https://github.com/tensorflow/tensorflow/commit/cd4f5840
- https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/common_runtime/gpu/gpu_managed_allocator.h#L23
- https://hpc.sjtu.edu.cn/ppmm15_uma_slides.pdf
- https://www.olcf.ornl.gov/wp-content/uploads/2017/01/SummitDev_Unified-Memory.pdf
- http://on-demand.gputechconf.com/supercomputing/2013/presentation/SC3120-Unified-Memory-CUDA-6.0.pdf
- http://on-demand.gputechconf.com/gtc/2016/presentation/s6216-nikolay-sakharnykh-future-unified-memory.pdf
