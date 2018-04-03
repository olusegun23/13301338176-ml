### Intel Open Questions
1. Is it possible to create PR to PaddlePaddle repo without using a fork?
   
   Yes, but only by contributors who have the write permission.  According to Github's permission management rules, if a user could create a PR without forking, it assumes that the user has the write permission to the main repo, thus s/he could git push his/her feature branch into the main repo.  However, as the number of contributors of PaddlePaddle grows, we have to formalize the permission management process -- new contributors start from the read permission only, after several rounds of PRs, they could get the write permission if a number of committers endorse his/her work.

2. What is kAnyLayout. How should we treat tensor in such layout when we want to transform it to some other layout, let say the MKLDNN one?

   https://github.com/PaddlePaddle/Paddle/issues/8298

3. Why only convolution and pooling have “use_cudnn” flag both in paddle v2 and fluid? Does it make sense to allow user to choose if cudnn has to be used only for these two OPs?
  
   cuDNN was designed to accelerate image processing related operators, in particular, convolution and pooling (see [CUDNN_LIBRARY.pdf](http://snurran.sics.se/hops/cudnn/CUDNN_Library.pdf) page-3). For these operators, we do prefer that our users could benefit from cuDNN; however, not all users have NVIDIA hardware required by cuDNN.  Therefore, we want that (1) our image related operators could work with and without cuDNN, and (2) our unit tests can verify that these operators work with and without cuDNN.  The operator attributes `use_cudnn` currently serves the purpose of specifying explicitly if the operators should use cuDNN during testing.

   As long as we are sure that any other operator could benefit from cuDNN, we should add the `use_cudnn` attribute to them too.

4. How does Python executes C++ code? Particularly, how C++ operations are registered to be available in Python
   
   1. Our [CMakeList.txt](https://github.com/PaddlePaddle/Paddle/blob/develop/python/CMakeLists.txt#L47) builds Fluid C++ code into core.so, so Fluid application developers could `import` it.

   1. We use [pybind](https://github.com/PaddlePaddle/Paddle/tree/develop/paddle/pybind) to expose C++ functionalities to Python.

   Please be aware that Fluid is conceptually different from TensorFlow or PyTorch in that a Fluid application program, when executed, doesn't do either training or inference; instead, it just exports itself into another format, a ProgramDesc protobuf message.  Then, the Python Fluid program calls C++ class Executor to run this message just like a (Python/Ruby/Java) interpreter executes a pre-compiled (Python/Ruby/Java) program.

5. We're going to add new DataLayout enum like kMKLDNN in paddle to avoid unnecessary transformations to MKLDNN format, which is required for this library to work efficiently. Are you OK with that?

   According to the answer to question 2, our DataLayout is not a general concept related with all Tensors, but only an additional piece of information for tensors that represent images. So, we are going to remove the `layout_` property from class Tensor.  However, it could be that [MKLDNN layout](https://github.com/intel/mkl-dnn/blob/master/src/common/c_types_map.hpp#L100) is conceptually different from  PaddlePaddle's `DataLayout`.  The solution could be:

   1. Rename DataLayout into FourDimImageTensorLayout.
   1. Move the property `FourDimImageTensorLayout layout_` out from class `Tensor`.
   1. Create a new class `ImageTensor` derived from `Tensor` and has the above property.
   1. Register this new Fluid data type `ImageTensor` in `framework.proto` as described in the above issue.

   Similar things could be done with MKLDNN layout:

   1. Create a new class `MKLDNNTensor` (if Intel friends think this is proper) derived from `Tensor`, and
   1. add the property `mkldnn_memory_format_t format_` into `MKLDNNTenosr`.

   All above are just a proposal. Maybe a discussion in an issue could involve more contributors and lead to a good solution.

6. Should we use both global use_mkldnn flag and per-OP use_ mkldnn flag as it is in paddle v2?

   Yes, we need both.  The global `use_mkldnn` flag is supposed to be used by our kernel-selection mechanism.  The per-operator `use_mkldnn` attribute is used in unit tests to have fine-grained control of kernel-selection.

7. In paddle v2 there are c++UTs for operators. In fluid it seems that there are only python UTs. Should we apply to that and create UTs for MKLDNN OPs in python?
   - Yes, you should only modify the 
   [`if core.is_compiled_with_cuda() and core.op_support_gpu(self.op_type):       places.append(core.CUDAPlace(0))`](https://github.com/PaddlePaddle/Paddle/blob/develop/python/paddle/v2/fluid/tests/op_test.py#L337), adding MKLDNNPlace. Then, the UTs for MKLDNN OPs in python would be created automatically.
    - Python UT actually call c++ core to do computation.

8. Can you define “Intel” label and use it with issues you find we (Intel) team should look at?
   - Sure. We already created it.

What should do to add MKLDNN kernel
 - https://github.com/PaddlePaddle/Paddle/issues/8305