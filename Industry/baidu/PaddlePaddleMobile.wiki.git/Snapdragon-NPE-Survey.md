- [Architecture](#architecture)
- [C++ API](#c-api)
- [Benchmark](#benchmark)
- [How to integrate in caffe2?](#how-to-integrate-in-caffe2)

## Architecture
![](https://user-images.githubusercontent.com/7845005/33125097-4ad7f65a-cfba-11e7-8635-996f8168255f.png)
- Convert model to DLC(Deep Learning Container) file.
   - Support Caffe, Caffe2, TensorFlow
- Use SNPE (Snapdragon Neural Processing Engine) to run the model.
- Support Snapdragon CPU, Adreno GPU, HexagonTM DSP (fixed point).

## C++ API
1. Get Available Runtime: CPU,GPU or DSP.

```c++
static zdl::DlSystem::Runtime_t Runtime;
if (zdl::SNPE::SNPEFactory::isRuntimeAvailable(zdl::DlSystem::Runtime_t::GPU)) {
  Runtime = zdl::DlSystem::Runtime_t::GPU;
} else {
   Runtime = zdl::DlSystem::Runtime_t::CPU;
}
```

2. Load Container (called load model)

```c++
std::unique_ptr<zdl::DlContainer::IDlContainer> container;
container = zdl::DlContainer::IDlContainer::open(dlc_file);
```
3. Set Network Builder Options Based on the Container

```c++
std::unique_ptr<zdl::SNPE::SNPE> snpe;
zdl::SNPE::SNPEBuilder snpeBuilder(container.get());
snpe = snpeBuilder.setOutputLayers({})
       .setRuntimeProcessor(runtime)
       .setUdlBundle(udlBundle)
       .setUseUserSuppliedBuffers(useUserSuppliedBuffers)
       .build();
```
4. Load Network Inputs

   Network inputs and outputs can be either user-backed buffers or ITensors (built-in SNPE buffers), but not both. The advantage of using user-backed buffers is that it eliminates an extra copy from user buffers to create ITensors. 
   - zdl::DlSystem::UserBufferMap& userBufferMap
   - zdl::DlSystem::ITensor

5. Execute the Network & Process Output

   ```c++
   zdl::SNPE::SNPE snpe;
   zdl::DlSystem::UserBufferMap& inputMap;
   zdl::DlSystem::UserBufferMap& outputMap;
   snpe->execute(inputMap, outputMap);

   // or use TensorMap 
   // bool execute(const zdl::DlSystem::TensorMap &input,
   //             zdl::DlSystem::TensorMap &output) noexcept;
   ```

6. Summary
   - zdl::DlContainer::IDlContainer
   - zdl::SNPE::SNPEBuilder
   - zdl::SNPE::SNPE
   - zdl::DlSystem::ITensor
     - zdl::SNPE::SNPEFactory::getTensorFactory().createTensor
   - zdl::DlSystem::TensorMap
   - zdl::DlSystem::UserBufferMap

## Benchmark
Not testing, the number is from the doc.

- Performance
  - Running on the GPU
    - Typically, 6X-10X speed compared with CPU
    - However, 4-6ms overhead for network execution on the GPU. Small network(less than 10ms on the GPU) run faster on CPU.

- Size

Lib                   | libSNPE.so | libSNPE_G.so | 
:-------------------: | :--------: | :-----------:|
arm-android-gcc4.9    | 3.4M       | 1.7M         |
aarch64-android-gcc4.9|8.2M        | 7.2M         |

##  How to integrate in caffe2？
- [SNPEOp](https://github.com/caffe2/caffe2/blob/master/caffe2/mobile/contrib/snpe/snpe_op.cc)

  - One input is DLC model buffer. 
  ```python
  with open('submodel.dlc', 'rb') as f:
    dlc = f.read()
  op = core.CreateOperator('SNPE', ['data_in'], ['data_out'],
         arg=[
             utils.MakeArgument("model_buffer", dlc),
             utils.MakeArgument("input_name", "data") 
         ]
     )
  ```
  - Wrap the C++ APIs of SPNE, then use these interfaces in SNPEOp: see [snpe_ffi.cc](https://github.com/caffe2/caffe2/blob/master/caffe2/mobile/contrib/snpe/snpe_ffi.cc)