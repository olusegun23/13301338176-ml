### What is TensorFlow Lite ?
TensorFlow Lite was proposed as a lightweight solution that focusses on mobile and other embedded devices. Using TensorFlow Lite on-device machine learning inference can be performed in an efficient manner. Efficiency here is measured in terms in terms of latency , size of the file and code footprint. Different techniques have been explored for achieving low latency: optimizing and tailoring kernels for mobile specifically, few pre-fused and pre-trained activations, and the proposal of quantized kernels that allow smaller and faster models.

### Why a mobile-specific library ?

In order to support fast inference and support machine learning processing (training and inference) on mobile and embedded devices, a light-weight framework is a necessity. There is also a need for:
- On-device real-time computer-vision.
- On-device spoken language understanding.
- Support for stronger user data privacy paradigms (user data resides strictly on-device).
- Serving ‘offline’ use cases when the device isn't connected to the internet.

To support the use cases mentioned above, a framework like TensorFlow Lite would be a requirement.

### Major contributions of TensorFlow Lite

TensorFlow Lite has the following new components on top of what TensorFlow supports:
1. A set of core operators, specifically tuned for mobile platforms (iOS and Android). These operators support both quantized an float operations and have few pre-fused activations to enhance accuracy. These can be used to create and run custom models. 
2. Custom operations can be defined in kernels by the developer.
3. A new model file format that uses the FlatBuffers format. FlatBuffers is an open-sourced cross-platform serialization library (similar to protocol buffers used in TensorFlow). FlatBuffers comes with few advantages over protocol buffers: no parsing step needed before accessing data, per-object memory allocation, lesser code footprint.
4. Mobile-optimized interpreter that focuses on making the speed and size of the app small. The interpreter uses static graph ordering, custom memory allocator and kernels that focus on making the loading, initialization and latency very efficient.
5. TensorFlow converter that converts TensorFlow models to the TensorFlow Lite model format.
6. Android Neural Networks library. This library can be leveraged for hardware acceleration if the device supports it.
7. Smaller in size than TensorFlow Mobile. TensorFlow Lite is < 300KB in size when all operators are linked and <= 200KB when using only the operators needed for standard supported models (MobileNet and InceptionV3).
8. Java and C++ API support

### Architecture overview

The TensorFlow Lite architecture is as follows:
![](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/g3doc/TFLite-Architecture.jpg)

At the moment it supports both Android and iOS applications.
The first step is to convert a trained TensorFlow model to TensorFlow Lite file format (.tflite) using the TensorFlow Lite Converter as mentioned above. This converted model file can now be used in the application.

For deploying the model, following are the important components as shown in the diagram above:
1. Java API: A wrapper around the C++ API (for Android).
2. C++ API: This component is responsible for loading the model file and invoking the interpreter for further processing and execution. The same library is used for Android and iOS.
3. Interpreter: This component executes the model using the defined kernels. The interpreter supports selective kernel loading and developers can also define their own custom kernels that can be used to perform the execution.
4. On few Android devices, the Interpreter can use the Android Neural Networks API for hardware acceleration or default to using a CPU.

### Next Sections
1. [Android Neural Network API](https://github.com/PaddlePaddle/Mobile/wiki/Android-NN-Survey)
2. [FlatBuffers](https://github.com/PaddlePaddle/Mobile/wiki/FlatBuffers)
3. TensorFlow for Poets
4. MobileNets (kexinzhao)
