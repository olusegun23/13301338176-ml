## How to Add Operators for new Library or Device

### Step 1. Write Operator and it's Kernels.
Please refer to this document:
[`new_op`](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/howto/dev/new_op_en.md)

The mechanism that an Operator choose a kernel is in this document. [`switch_kernel`](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/switch_kernel.md)

### Step 2. Register the Operator and it's kernels to framework

After writing the components described in [`new_op`](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/howto/dev/new_op_en.md). We should register the Operator and its kernels into the framework.

When register Operator kernels, we should consider the type of the kernels.

We use `OpKernelType` to describe the type of an OperatorKernel.

```cpp
struct OpKernelType {
  platform::Place place_;
  platform::Library library_;
  proto::DataType data_type_;
  framework::Layout layout_;
};
```

The meaning of each filed for an OpKernelType is in this document: [`operator_kernel_type`](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/operator_kernel_type.md)

Take `conv_2d` as an example:

- Firstly, [register conv2d operator](https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/operators/conv_op.cc#L304)

 ```cpp
 REGISTER_OP(conv2d, ops::ConvOp, ops::Conv2DOpMaker, conv2d_grad,
            ops::ConvOpGrad);
 ```

- Secondly, Register Operator kernels with their library/place/data_type. [Refs](https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/operators/conv_cudnn_op.cu.cc#L318)

	```cpp
	REGISTER_OP_KERNEL(conv2d, CUDNN, ::paddle::platform::CUDAPlace,
	                   paddle::operators::CUDNNConvOpKernel<float>,
	                   paddle::operators::CUDNNConvOpKernel<double>);
	REGISTER_OP_KERNEL(conv2d_grad, CUDNN, ::paddle::platform::CUDAPlace,
	                   paddle::operators::CUDNNConvGradOpKernel<float>,
	                   paddle::operators::CUDNNConvGradOpKernel<double>);
	
	REGISTER_OP_KERNEL(conv3d, CUDNN, ::paddle::platform::CUDAPlace,
	                   paddle::operators::CUDNNConvOpKernel<float>,
	                   paddle::operators::CUDNNConvOpKernel<double>);
	REGISTER_OP_KERNEL(conv3d_grad, CUDNN, ::paddle::platform::CUDAPlace,
	                   paddle::operators::CUDNNConvGradOpKernel<float>,
	                   paddle::operators::CUDNNConvGradOpKernel<double>);
	```

In the code above:

 - `conv2d` is the type/name of the operator
 - `CUDNN` is `library`
 - `::paddle::platform::CUDAPlace` is `place`
 - template parameter `float/double` on `CUDNNConvOpKernel<T>` is `data_type`.