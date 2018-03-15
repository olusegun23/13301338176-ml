- [Architecture](#architecture)
- [C++ API](#c-api)
- [Optimization](#optimization)
- [How to Integrate Android Neural Networks API](#how-to-integrate-android-neural-networks-api)


## Architecture
- Architecture Introduction
  
  See the architecture graph: https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/lite
  ![](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/g3doc/TFLite-Architecture.jpg)
  - Lite Converter
    Also called freeze Graph, it will merge the checkpoint values with the graph structure.
  - Android APP
    - Jave API
    - C++ API
    - Interpreter: The main executive engines
    - Android Neural Network API.

- What is the relationship between TensorFlow and TfLite?

  There is no relationship between TensorFlow and TfLite. TfLite is another lightweight inference framework.
## C++ API
### [Programing Model](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/g3doc/apis.md)
The simple usage is as follows:
```cpp
// 1. Load Model
tflite::FlatBufferModel model(path_to_model);

// 2. Init and Build Interpreter
tflite::ops::builtin::BuiltinOpResolver resolver;
std::unique_ptr<tflite::Interpreter> interpreter;
tflite::InterpreterBuilder(*model, resolver)(&interpreter);

// 3. Resize input tensors, if desired.
// Allocate Tensors and fill `input`.
interpreter->AllocateTensors();
float* input = interpreter->typed_input_tensor<float>(0);

// 4. Inference
interpreter->Invoke();

// 5. Read the output
float* output = interpreter->type_output_tensor<float>(0);
```
### Operator Pruning and BuiltinOpResolver
- [BuiltinOpResolver](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/kernels/register.h#L26) 
   - The regular usage will require the developer to use the BuiltinOpResolver, which has [many operators](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/kernels/register.cc#L51).
- Operator pruning
  - [Users can rewrite other OpResolver to prune the operators.](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/kernels/register.cc)
### Interpreter && InterpreterBuilder

## Optimization 
  - Optimizing the kernels
     - NEON
     - multi-threads
     - All the kernel available support int8 and float32
  - Pre-fused activations: for example, pre-fuse bias adding and activation: https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/kernels/internal/optimized/optimized_ops.h#L281
  - use [FlatBuffers](https://github.com/google/flatbuffers) instead `protobuf-lite` to serialize.
  - There is a separate optimization for mobilenet
https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/lite/kernels/internal/optimized
  - a simple arena memory management
https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/simple_memory_arena.h
  - NHWC data format

## How to Integrate Android Neural Networks API
The [Android Neural Networks API (NNAPI)](https://developer.android.com/ndk/guides/neuralnetworks/index.html) is an Android C API designed for running computationally intensive operators for machine learning on mobile devices. Tensorflow Lite is designed to use the NNAPI to perform hardware-accelerated inference operators on supported devices. 

For the details about `NNAPI`, you can refer to [Android NN survey](https://github.com/PaddlePaddle/Mobile/wiki/Android-NN-Survey). The way how to integrate `NNAPI` in TfLite is described as following.

- In C++ API, the TfLite will init and build `Interpreter`. This process will detect whether the `NNAPI` existed or not.

```c++
// 2. Init and Build Interpreter
tflite::ops::builtin::BuiltinOpResolver resolver;
std::unique_ptr<tflite::Interpreter> interpreter;
tflite::InterpreterBuilder(*model, resolver)(&interpreter);
``` 

```c++
Interpreter::Interpreter(ErrorReporter* error_reporter)
    : arena_(kDefaultArenaAlignment),
      persistent_arena_(kDefaultArenaAlignment),
      error_reporter_(error_reporter ? error_reporter
                                     : DefaultErrorReporter()) {
  context_.impl_ = static_cast<void*>(this);
  context_.ResizeTensor = ResizeTensor;
  context_.ReportError = ReportError;
  context_.AddTensors = AddTensors;
  context_.tensors = nullptr;
  context_.tensors_size = 0;
  context_.gemm_context = nullptr;
  // Reserve some space for the tensors to avoid excessive resizing.
  tensors_.reserve(kSlotsToReserve);
  nodes_and_registration_.reserve(kSlotsToReserve);
  next_allocate_node_id_ = 0;
  UseNNAPI(false);
}
```

```c++
TfLiteStatus NNAPIDelegate::Invoke(Interpreter* interpreter) {
  if (!nn_model_) {
    // Adds the operations and their parameters to the NN API model.
    TF_LITE_ENSURE_STATUS(BuildGraph(interpreter));
  }

  ANeuralNetworksExecution* execution = nullptr;
  CHECK_NN(ANeuralNetworksExecution_create(nn_compiled_model_, &execution));

  // Currently perform deep copy of input buffer
  for (size_t i = 0; i < interpreter->inputs().size(); i++) {
    int input = interpreter->inputs()[i];
    // TODO(aselle): Is this what we want or do we want input instead?
    // TODO(aselle): This should be called setInputValue maybe to be cons.
    TfLiteTensor* tensor = interpreter->tensor(input);
    CHECK_NN(ANeuralNetworksExecution_setInput(
        execution, i, nullptr, tensor->data.raw, tensor->bytes));
  }
  // Tell nn api where to place final data.
  for (size_t i = 0; i < interpreter->outputs().size(); i++) {
    int output = interpreter->outputs()[i];
    TfLiteTensor* tensor = interpreter->tensor(output);
    CHECK_NN(ANeuralNetworksExecution_setOutput(
        execution, i, nullptr, tensor->data.raw, tensor->bytes));
  }
  // Currently use blocking compute.
  ANeuralNetworksEvent* event = nullptr;
  CHECK_NN(ANeuralNetworksExecution_startCompute(execution, &event));
  CHECK_NN(ANeuralNetworksEvent_wait(event));
  ANeuralNetworksEvent_free(event);
  ANeuralNetworksExecution_free(execution);

  return kTfLiteOk;
}
```