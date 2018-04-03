
## Majel VS Eigen

Give an example how to do addition on GPU.

### Majel

```c++
set_place(GpuPlace());
{
    const int dim1 = 2;
    const int dim2 = 3;
    auto darray1 = fill(make_ddim({dim1, dim2}), 2.0f);
    auto darray2 = fill(make_ddim({dim1, dim2}), 5.5f);

    auto darray3 = ones(make_ddim({dim1, dim2}), 2.0f);

    binary_op(plus(), darray1, darray2, darray3);

    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            ASSERT_EQUAL(darray3[make_ddim({i, j})], 7.5f);
        }
    }
}
```

### Eigen

Please check out https://github.com/PaddlePaddle/Paddle/wiki/A-Survey-and-Taxonomy-of-Eigen
to find out more details about Eigen.

```c++
  Tensor<float, 3> in1(Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Tensor<float, 3> in2(Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Tensor<float, 3> in3(Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Tensor<float, 3> out(Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  in1.setRandom();
  in2.setRandom();
  in3.setRandom();

  std::size_t in1_bytes = in1.size() * sizeof(float);
  std::size_t in2_bytes = in2.size() * sizeof(float);
  std::size_t in3_bytes = in3.size() * sizeof(float);
  std::size_t out_bytes = out.size() * sizeof(float);

  float* d_in1;
  float* d_in2;
  float* d_in3;
  float* d_out;
  cudaMalloc((void**)(&d_in1), in1_bytes);
  cudaMalloc((void**)(&d_in2), in2_bytes);
  cudaMalloc((void**)(&d_in3), in3_bytes);
  cudaMalloc((void**)(&d_out), out_bytes);

  cudaMemcpy(d_in1, in1.data(), in1_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_in2, in2.data(), in2_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(d_in3, in3.data(), in3_bytes, cudaMemcpyHostToDevice);

  Eigen::CudaStreamDevice stream;
  Eigen::GpuDevice gpu_device(&stream);

  Eigen::TensorMap<Eigen::Tensor<float, 3> > gpu_in1(d_in1, Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Eigen::TensorMap<Eigen::Tensor<float, 3> > gpu_in2(d_in2, Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Eigen::TensorMap<Eigen::Tensor<float, 3> > gpu_in3(d_in3, Eigen::array<Eigen::DenseIndex, 3>(72,53,97));
  Eigen::TensorMap<Eigen::Tensor<float, 3> > gpu_out(d_out, Eigen::array<Eigen::DenseIndex, 3>(72,53,97));

  gpu_out.device(gpu_device) = gpu_in1 + gpu_in2 * gpu_in3;

  assert(cudaMemcpyAsync(out.data(), d_out, out_bytes, cudaMemcpyDeviceToHost, gpu_device.stream()) == cudaSuccess);
  assert(cudaStreamSynchronize(gpu_device.stream()) == cudaSuccess);

  for (int i = 0; i < 72; ++i) {
    for (int j = 0; j < 53; ++j) {
      for (int k = 0; k < 97; ++k) {
        VERIFY_IS_APPROX(out(Eigen::array<Eigen::DenseIndex, 3>(i,j,k)), in1(Eigen::array<Eigen::DenseIndex, 3>(i,j,k)) + in2(Eigen::array<Eigen::DenseIndex, 3>(i,j,k)) * in3(Eigen::array<Eigen::DenseIndex, 3>(i,j,k)));
      }
    }
  }

  cudaFree(d_in1);
  cudaFree(d_in2);
  cudaFree(d_in3);
  cudaFree(d_out);
```

## How to use Eigen in TensorFlow

```c++
typedef float T;
Tensor my_mat(...built with Shape{rows: 3, cols: 5}...);
auto mat = my_mat.matrix<T>();    // 2D Eigen::Tensor, 3 x 5.
auto mat = my_mat.tensor<T, 2>(); // 2D Eigen::Tensor, 3 x 5.
auto vec = my_mat.vec<T>();       // CHECK fails as my_mat is 2D.
auto vec = my_mat.tensor<T, 3>(); // CHECK fails as my_mat is 2D.
auto mat = my_mat.matrix<int32>();// CHECK fails as type mismatch.
```

