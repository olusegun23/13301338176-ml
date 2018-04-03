Paddle的整体模块如下图所示:

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/40d26306f4c06311401d0d14b17479fb/raw/60da6a0e9ece909a2d78a56a65b8ac05ad208f70/PaddleArchiGlobal)

*  Math中有各种矩阵和向量的实现，矩阵和向量基类是BaseMatrix。
* 矩阵和向量组成了神经网络的参数Parameter和神经网络层的输入和输出Arguments。
* Layer使用Argument和Parameter完成计算。
* GradientMachine是一个把神经网络各个层组合在一起调用的类型。这是个基类，具有神经网络常见的`forward`,`backward`函数，并且处理了单机多线程和多显卡的功能。
* Trainer调用`GradientMachine`计算出参数的梯度。
* ParameterUpdater将梯度优化到参数值上。
* Trainer优化的拓扑结构是Python端的config_parser.py程序生成的。

目录:

   * [Math](#math)
      * [MemoryHandle](#memoryhandle)
      * [Matrix/Vector](#matrixvector)
         * [BaseMatrixT](#basematrixt)
         * [VectorT](#vectort)
         * [Matrix](#matrix)
   * [Parameter/Argument](#parameterargument)
      * [Parameter](#parameter)
      * [Argument](#argument)
   * [Layer](#layer)
      * [使用paddle::Function重构Layer](#使用paddlefunction重构layer)
      * [Paddle中的细粒度层--MixedLayer](#paddle中的细粒度层--mixedlayer)
      * [RNN单元](#rnn单元)
      * [Recurrent Layer Group](#recurrent-layer-group)
   * [GradientMachine](#gradientmachine)
      * [RecurrentGradientMachine](#recurrentgradientmachine) @caoying
         * [变量介绍](#变量介绍)
         * [计算思路](#计算思路)
         * [AgentLayer](#agentlayer)
         * [Frame流程图](#frame流程图)
         * [beam search代码](#beam-search代码)
         * [重构想法](#重构想法)
   * [ParameterUpdater](#parameterupdater)
   * [ParameterOptimizer](#parameteroptimizer)
   * [Trainer](#trainer)


# Math

Math模块是Paddle的数学计算模块。这个模块里主体分为两个部分。

* MemoryHandle: 申请，管理内存和显存的模块。Paddle中的所有计算都是对MemoryHandle的操作。
* Matrix/Vector: Paddle中实现计算逻辑的类型。本质上是MemoryHandle的一个view。

简单的说，这段代码的逻辑为:

```cpp
class MemoryHandle {
public:
   void* buffer;
   size_t size;
   ...
};

class Matrix {
public:
  MemoryHandlePtr memory;
  
  void do_XXX(...);
  void add(...);
  void mul(...);
}
```

下面分子模块介绍Math的细节。

## MemoryHandle

MemoryHandle本质上是对一段内存的抽象。包括这段内存的地址和大小。这段内存可以是申请在普通内存上，也可以是申请在GPU设备上。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/34f01e4ca2aa11b286f5c58f626b8f6c/raw/d71086662337b450ecf42d2c628c092e1ef355b0/)

MemoryHandle实际上被Matrix与Vector使用，而Matrix实际上是作为MemoryHandle的view，其中的操作都是直接对这段内存的修改。

值得注意的是，MemoryHandle本身申请的内存是无类型的，这段内存具体是整数，还是浮点数，都由上层的使用者决定。甚至一个MemoryHandle可能会被多个内存解释。譬如稀疏矩阵中，三个数组(两个整数数组做index，一个浮点数数组表示数据)，都使用同一个MemoryHandle申请。

而在MemoryHandle之下，使用了一些工具类帮助申请内存/显存，他们是`StorageEngine`,`PoolAllocator`,`Allocator`。这些类只被MemoryHandle使用。

## Matrix/Vector

Paddle中Matrix和Vector相关模块整体结构图如下:

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/132c11fb589c414fd35ec366fe70c433/raw/d79c584f734c0603901287f25cb2220d929dd117/vec_mat.dot)

### BaseMatrixT

Paddle中所有矩阵与向量相关的类型，都继承自`BaseMatrixT`。BaseMatrixT是一个模板类型，T为可以接受的数据类型。在这个`BaseMatrixT`中并没有维护内存如何申请，而是直接使用了裸指针访问数据。

```cpp
template <typename T>
class BaseMatrixT {
public:
  T* data_;
  bool useGPU_;
  ...
};
```

BaseMatrixT抹平了CPU和GPU实现差异，实现了一些计算函数。在BaseMatrixT中实现可以用同样的代码同时实现CPU和GPU函数。譬如对于Relu函数，BaseMatrixT中的实现为:

```cpp
DEFINE_MATRIX_BINARY_OP(Relu, b = a > 0.0f ? a : 0.0f);
template <typename T>
void BaseMatrixT<T>::relu(BaseMatrixT& b) {
  applyBinary(binary::Relu<T>(), b);
}
```

同时，BaseMatrixT并不是支持所有的计算类型，而是将需要使用的类型在BaseMatrix.cu中进行了特化。这样避免了代码膨胀。

```cpp
template class BaseMatrixT<real>;
template class BaseMatrixT<int>;
```

### VectorT

在BaseMatrix之上，Paddle的向量`VectorT`继承了`BaseMatrixT`，进而继承了BaseMatrix中实现的一些计算函数。在`VectorT`中实现了一些向量独有的函数，并且使用MemoryHandle作为成员变量来管理内存。VectorT的两个子类分别是`CpuVectorT`和`GpuVectorT`，分别实现了CPU和GPU相关的特殊操作。在Vector中，typedef出了两大类型。

```cpp
template <typename T>
class VectorT: public BaseMatrixT<T> {
public:
  MemoryHandlePtr mem;
  VectorT(): BaseMatrixT<T>(data=mem.data_) {}
  ...
};

typedef VectorT<int> IVector;
typedef VectorT<real> Vector;
```

### Matrix

Matrix支持的数据类型就比较简单，不像Vector支持float和int两种数据类型，Matrix只支持float，也就是直接继承自`BaseMatrix<float>`。当矩阵支持的稀疏/稠密类型比较多，他们的类型分别是:

* CpuMatrix: CPU稠密类型的矩阵，即所有的元素在内存中是稠密排布的，中间没有间隔。(没有补零等等)。
* GpuMatrix: GPU稠密类型的矩阵，所有元素在显存中是稠密排布的，中间没有间隔。
* CpuSparseMatrix: CPU稀疏类型矩阵。内部存储三段内存，两组整数数组和一组浮点数数组。CpuSparseMatrix支持两种稀疏表示方式，即CSR和CSC两种稀疏格式。具体参考[wiki](https://en.wikipedia.org/wiki/Sparse_matrix)。无论以何种方式表示稀疏矩阵，两个整数数组表示这个矩阵中哪一个元素非0，浮点数组表示这个非零元素的值。 稀疏矩阵可以不显示的设置稀疏元素值，即浮点数数组可以为NULL。如果为NULL则表示，非零值元素的值为1.
* GpuSparseMatrix: 与CpuSparseMatrix类似，只是计算操作位于GPU设备中。
* SparseRowMatrix: SparseRowMatrix是Paddle为了实现神经网络稀疏更新而特殊创造的Matrix。他往往是一个神经网络的参数矩阵。而这个参数矩阵并不包含全量的参数，而是只包含这个参数某些行的稀疏矩阵。但是，这个稀疏矩阵的粒度是行，即要么某一行全为0，要么某一行全有值。并且，使用连续的内存表示所有非稀疏的矩阵行，再加上一个`index`对象表示哪行是空。


# Parameter/Argument

Parameter和Arguments表示神经网络中所有的数据和参数。其中Parameter表示神经网络中层与层之间的连接参数，而Argument表示每一层的输入和输出。即Parameter表示下图中的黄色连线，而Argument表示下图中的的输入和输出(Input, Output)。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/e001be14baab551e52fa5457cee088c2/raw/a675f32e37f2a00d934d995b03425cded4712bc4/param_arg.dot)

Parameter与Argument并不只保存了参数值，还同时保存了梯度，动量等信息。

而Parameter/Argument与`Math`模块的关系为:

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/5438a0e148e94e59409cdb589f2df408/raw/82ab1b1ebfe230eea4f1e4aa11d02ee07a3518a7/param_arg_archi.dot)

## Parameter

Parameter是一个字典，他的Key是参数类型(值，梯度，还是动量？), Value是一个`Vector`。即Paddle中的参数值全部拍平了，是一个一维向量。简单的代码表示为:

```cpp
enum ParameterType {
  PARAMETER_VALUE,
  PARAMETER_GRADIENT,
  PARAMETER_MOMENTUM,
  ...
};

struct Parameter {
  map<PrameterType, VectorPtr> buffers_;
};
```

在Parameter类中，主要维护了如下功能:

* 参数的创建和析构。
* 参数的共享问题，稀疏问题。比如某一层的参数初始化的时候是稀疏的，或者可能被多个CPU线程所共享。
* 参数初始化工作。
* 参数的载入和保存。
* 参数更新过程的计数器。譬如某一个参数在神经网络中使用了多次，需要维护一个计数器确定所有被使用的参数都已经有过更新。


## Argument

Argument类是Paddle中表示神经网络每一层输入和输出的参数。他主要包含了输入输出的值和梯度，时间序列信息。其他信息的用途，大部分为各个Layer实现时，每个Layer根据自身需要定义的用途。简单的实现代码如下:

```cpp
struct Argument {
  MatrixPtr value;
  MatrixPtr grad;
  IVectorPtr sequenceStartPosition;
  IVectorPtr subSequenceStartPosition;
  ...
};
```

* 输入值信息，value。Value一个神经网络某一层的某一个输入的值。他是一个二维矩阵，矩阵的高度为这一次输入中时间步的数量。如果这个输入不是一个时间序列(Sequence)，那么该矩阵高度，即为batch size。这个矩阵的宽即为全部的每一条数据。譬如输入数据是一个有3个RGB channel的图片，图片为28\*28像素。那么这个图片就很单纯的拍平了成一个向量传递给这个Layer。即矩阵宽度为 28\*28\*3。
* 输入的梯度信息，grad。Grad是神经网络某一层的某一个输入的梯度。他是由使用该Argument的层，在反向传播的时候负责设置的。
* sequenceStartPosition，序列数据中每一个条样本开始和结束的位置信息。这里保存了一个整数数组，这个数组是value矩阵的slices。例如sequenceStartPosition=[0, 7, 9, 13]，那么表示该输入有三个样本构成，第一个样本为value[0:7]，第二个样本为value[7:9]，第三个样本为value[9:13]
* subSequenceStartPosition，子序列中每一个样本开始和结束的位置信息。子序列信息格式和sequenceStartPosition一致，只是反映的是子序列的信息。

Argument中还有其他的一些项目，这些项目基本上会被某一些Layer使用，而实用的方法这些项目的含义和使用这个Argument的Layer有关。譬如

```cpp
struct Argument {
...
IVectorPtr ids;
...
};
```
对于Argument.ids这个对象。他是一个整数数组，在一些计算Cost的Layer中，他表示监督学习里，数据中实际的label值。而在Embedding相关的Layer中(TableProjection)，表示一句话中某一个词在词典中的偏移值(词ID)。

# Layer

Layer是Paddle中的一个基类，所有可以被用户使用配置的层，都继承自Layer。而Layer使用的了Argument做为输入和Parameter当做参数。Layer运行在**某一个**设备上，也就是Layer本身只是单线程的。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/81f841da12c385753e5536acef7c27e0/raw/2bcf96c1a7f7749b754a9fd95a83d388fcdaf2c9/layer_base.dot)

Paddle中的Layer是一个基类，用户实现一个Layer的时候，实际上只需要实现`forward`和`backward`两个函数。而具体如何实现每一个层，Paddle中并没有具体要求。

```cpp
class Layer {
public:
  Argument output_;
  std::vector<LayerPtr> inputs_;
  std::vector<ParameterPtr> inputParams_;
  ...
};
```

实现Paddle的Forward和Backward函数时，将计算结果保存到`output_`变量中即可。而这一层的输入，使用`inputs_`变量来获取输入的层是哪些，在使用`inputs_[i]->output`来获得输入。而对应每一个输入的参数，使用`inputParams_`来获取。

值得注意的是，Paddle将`Activation`和`dropout`看成Layer基类中的一个功能。并没有特殊的层去做Activation和dropout，而是activation和dropout是每一个Layer的属性。

```cpp
void backward(std::function<void(Parameter* )> callback);
```

同时，每一个层的Backward函数，并不仅仅计算完该参数的梯度，还增加了参数(Parameter)中的『更新计数』。当参数中的更新计数等于这个参数在拓扑结构的引用次数后，backward函数会通知调用者，某一个参数反向传播已经计算完成。

Paddle中有一些特别的Layer和特别的实现需要讲解，他们是:

* 我们目前使用`paddle::Function`来重构Layer的实现，进而优化Paddle的Layer包
* **MixedLayer**: Paddle中实现基于OP和Projection的轻量级配置层
* **Recurrent Group**: Paddle中实现自定义RNN功能的层。


## 使用`paddle::Function`重构Layer

之前说明，Layer中并没有规定用户使用什么方式实现这个Layer。只需要实现`forward`和`backward`两个函数，将计算结果保存在本地的`Argument`中即可。这有如下几个问题:

* 有一些重复性质的代码。譬如输入输出合法性的检查、每一层输入输出形状的推到，将拍平了的Parameter从向量转换成实际使用的参数矩阵。
* Layer是一个有状态的对象。即将每一个Layer的输出保存了本地作为一个data member。
	* 这不方便全局对Layer输出的优化。譬如将所有Layer的输出统一申请内存，将部分Layer的输出从显存转移至内存，以便节约显存等等。
* 区分了Argument和Parameter，即区分了层的输入和参数。但实际上，输入和参数在神经网络里都是一段内存，不应该有所区分。
  * 实现NeuralArt比较困难。
  * 有很多操作实现了很多遍。譬如MulOperator和MulProjection。

在当前的重构过程中，我们正在使用`paddle::Function`来实现Paddle的`forward`与`backward`函数。`paddle::Function`是一个无状态的函数，他的模块图如下图所示:

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/49abce22c7e82ce4f3329e7b6750d958/raw/c58cc6f750334cb5bf688d914f40bc4be0d5cc4c/function.dot)

Function的输入和输出都是`BufferArgs`类型，而BufferArgs类型实际上是`BufferArg`的数组。`BufferArg`直接由`Parameter`和`Argument`转换而来，并在本地没有记录计算状态。这修正了`paddle::Layer`中的问题。

并且使用`Function`类型，可以对层的输入和输出合法性进行统一的检查，预估计算某一个函数需要的`flops`等等。给了Layer的一种统一实现的方式。

在使用`paddle::Function`重构的Layer中，`forward`函数与`backward`函数，分别由若干个`paddle::Function`组成。在forward和backward的过程中，依次调用对应的函数。

## Paddle中的细粒度层--MixedLayer

Paddle整体是一个基于Layer配置的神经网络框架。为了支持更细粒度的神经网络配置，支持配置op和projection，Paddle提供了MixedLayer。

MixedLayer和其他Layer不同，使用的输入类型不直接是其他Layer本身，而是其他Layer的projection或者operation。其他Layer的projection和operation的结果依次相加到MixedLayer的输出中。其中:

* Projection是指含有参数的一种变换，Projection中的参数是可学习的。
* Operation是没有参数的变换，他的输入为两个Layer的输出。Operation表示某两个Layer直接进行操作。

使用方式如下:

```python
with mixed_layer(size=200) as mix:
  mix += table_projection(input=word_id)
  mix += mul_operator(a=weight1, b=weight2)
```

而在MixedLayer实现的过程中，添加了两个轻量级类型`Projection`和`Operation`。他们也都有`forward`和`backward`函数。在MixedLayer中，forward与backward依次调用内部projections和operations的`forward`与`backward`函数即可。

值得注意的是: 目前来看，直接提供一个细粒度的操作层远比提供一个特殊的Layer在这个特殊Layer中配置细粒度操作来的方便，直观。`Projection`和`Operation`理应在重构中去掉。
  
## RNN单元

特点：无padding，算的快。

简单实现：sequence by sequence，需要27次计算

![](https://github.com/PaddlePaddle/Paddle/wiki/refactor_discuess/sequence_compute.png)

**并行实现**：batch实现，只需7次计算

![](https://github.com/PaddlePaddle/Paddle/wiki/refactor_discuess/batch_compute.png)

- 原始输入数据结构
  - sequence matrix: [C1_s ... Cn_s | ...... | C1_t ... Cn_t]
  - Cn_s： 序列s的第n个时刻
  - 包含多个不定长序列
  - 每个序列在内存中连续存储
- RNN计算的数据结构
  - batch matrix: [C1_s ... C1_t | ...... | Cn_s ... Cn_t]
  - 相同时刻在内存中连续存储
  - C1_s ... C1_t：序列s到序列t的第一个时刻

SequenceToBatch的步骤：
- 计算每个序列长度并排序
- 计算每个batch
  - 每个batch数据的长度等于包含当前时刻的序列个数
  - batch中每个单元对应到sequence中每个时刻的索引向量
 
下面的动图表示reverse=False/True时，SequenceToBatch的步骤。

![](https://github.com/PaddlePaddle/Paddle/wiki/refactor_discuess/Sequence2Batch.gif)

注意：Paddle中的RNN计算都遵循如上并行实现的想法，不管是封装好的RNN模块，如LSTM、GRU等，还是可以灵活定义的Recurrent Layer Group。

## Recurrent Layer Group

Paddle实现了在RNN中，对所有的时间步，定义任意类型的子网络。用户只需定义RNN在一个时间步内完成的计算，PaddlePaddle负责完成信息和误差在时间序列上的传播。但是在Paddle的上层中，并没有对Recurrent Layer Group本身有特殊的接口。即本身也是继承自Layer，也是表示神经网络中的一层。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/7cc4fdffe8e55bb213227c0ca4e31330/raw/2705f494ff52a24c80c095eaddb215628c77f505/ReurrentLayerGlobal.dot)


在Recurrent Layer Group中，维护了一个`NeuralNetwork`（在下面的`RecurrentGradientMachine`中详细介绍），也就是在RNN每个时间步中的具体操作。而对神经网络的输入，Recurrent Layer Group借助`ScatterAgentLayer`将输入按照每个时间步打散。对于RNN的输出，再借助于`GatherAgentLayer`按照将各个时间步的输出聚合成一个输出，传递给其他Layer。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/9ed1c30a4203205084208d3a03ffe4f8/raw/beffa1baaa9385110717b0984ef4b468971365f2/rnn_global.dot)

有`ScatterAgentLayer`和`GatherAgentLayer`可以针对每一个时间步，进行重复的操作。譬如，如上图所示，对于每一个时间步，均经过了同一个FC Layer。但是只有`ScatterAgentLayer`和`GatherAgentLayer`是不能实现RNN的，因为在不同时间步中，层与层之间是不能够通信的。而实现一个最简单的`Recurrent Layer Group`，需要跨时间步的通信。对于这种情况，Paddle使用了一种特殊的Layer完成，即『Memory』。

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/reyoung/d044787254f7872000b4e2da8a7a6854/raw/d9d7e08dbcc9db6a4e0ded9201a81ee31958e957/rnn_global_mem.dot)

Memory这个Layer可以跨越时间步，一些Layer可以将自己暂存到Memory中，而在下一个时间步中，将暂存在Memory的输出当做这个Layer的输入。

# GradientMachine

## RecurrentGradientMachine

### 变量介绍
代码参见[RecurrentGradientMachine.h](https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/gserver/gradientmachines/RecurrentGradientMachine.h#L294)
- `InFrameLines`/`OutFrameLines`：即`config_parser.py`的`in_links`和`out_links`
  - `LinkName`：RNN Group展开后出现的层名
  - `LayerName`：实际存在的层名
- `memoryFrameLines`：即配置中的memory记忆单元
  - `rootLayer`：memory的输入是外部网络中的layer
  - `biasLayer`：memory的输入是设定的某个值，如0
  - `bootLayer`：指rootLayer或者biaslayer
- `frames_`：RNN Group展开后的frame集合
  - 每个frame可看作一个子网络，包含Group中所有的Layer
  - 以batch方式进行，frame的个数是序列的长度或subseq的个数

### 计算思路

- 总体思路：序列中的每个frame依次走过每个Layer，前一个frame走完Group中所有的Layer后，后一个frame再走，且后一个frame能引用前一个frame的若干输出(memory)
- 前向计算思路：
  1. 计算frames的长度设为N：`getSeqLengthAndStart`函数
  1. 创建N个子网络： `resizeOrCreateFrames`函数
  1. 创建InFrameLine的batchIndex（均相同）: `createInFrameInfo`函数
  1. MemoryFrameLine如果有rootLayer，为其创建batchIndex（不一定相同）： `createMemoryFrameInfo`函数
  1. 输出的序列格式和输入的一样：`copyIdAndSequenceInfo`函数 
  1. 连接展开后各子网络间的`InFrameLine`/`OutFrameLine`/`MemoryFrameLine`
  1. 记忆单元对初始的输入进行前向计算
  1. 按展开后的Group，各frame依次进行前向计算
- 后向计算思路：
  1. 按展开后的Group，各frame依次进行反向计算
  1. 记忆单元对初始的输入进行反向计算

### AgentLayer

共有六种：
- `ScatterAgentLayer`/`SequenceScatterAgentLayer`：（拆分输入序列）
  - Forward：取出当前Frame需要的input片段
  - Backward：将每个Frame的梯度对应传回给input片段
  - 区别：前者取一个word，后者取一个subseq
- `AgentLayer`/`SequenceAgentLayer`:（拆分记忆序列）
  - Forward: 取出当前Frame需要的memory片段，无Backward
  - 区别：前者记忆num个word，后者记忆num个seq
- `GatherAgentLayer`/`SequenceGatherAgentLayer`：（合并输出序列）
  - Forward：将每个Frame计算出来的output合并成一个新的序列
  - Backward：将新序列的梯度拆分给每个Frame的output
  - 区别：前者拼接word，后者拼接subseq

### Frame流程图

假设一条Sequence含有2个word
- 前向流程图如下：需要额外做2次input的scatterAgent，2次memory的Agent，和1次output的GatherAgent

 <img src = "https://github.com/PaddlePaddle/Paddle/wiki/refactor_discuess/frame_forward.png" width=500>

- 后向流程图如下：需要额外做2次input的scatterAgent，和1次output的GatherAgent

 <img src = "https://github.com/PaddlePaddle/Paddle/wiki/refactor_discuess/frame_backward.png" width=500>

### beam search代码

待补充

### 重构想法
[RecurrentGradientMachine.h](https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/gserver/gradientmachines/RecurrentGradientMachine.h)和[RecurrentGradientMachine.cpp](https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/gserver/gradientmachines/RecurrentGradientMachine.cpp)是`recurrent_group`实现的C++代码，目前存在的问题如下，希望重构后能解决：
1. 这部分代码相比其他的layer来说，非常复杂且难以看懂，缺乏总体的设计文档。
2. `recurrent_group`**很难扩展到更加灵活的RNN结构**。
   - 之前增加的双层RNN相关代码，是嵌在`RecurrentGradientMachine.cpp`的各个函数中，写起来和调试起来都比较费劲。
   - 虽然有[recurrent_group的约定文档](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/howto/deep_model/rnn/recurrent_group_cn.md#双层rnn的使用)，但还是特别饶和晕。
3. `beam search`有一些自定义的剪枝方法，包括`BeamSearchControlCallbacks`、`BeamSearchStatisticsCallbacks`等class。这些方法是用于特定业务，使用场景少，且一直没有文档或示例，理解和维护代码起来很困难。是删掉还是维护文档？

# GradientMachine

GradientMachine是Paddle中对于神经网络的一种抽象，即该数据类型可以计算出`Gradient`，进而将计算后的结果放入`Parameter`中即可。

```cpp
class GradientMachine {
public:
   GradientMachine(Topology topo);

   map<std::string, ParameterPtr> parameters_;
   
   std::vector<Argument> forward(std::vector<Argument> args);
   
   void backward(std::function<void(Parameter*)> updateCallback);
};
```

一个GradientMachine一般用来计算一个神经网络的拓扑结构。进而，根据拓扑结构的形态，GradientMachine会创建一些`parameters_`，而`forward`根据输入的`args`和本地的参数，计算神经网络的前馈，而`backward`函数根据之前前馈的结果，计算出各个参数的梯度，并将各个参数的梯度保存在`parameters_`中。

GradientMachine只是一个接口。这个接口有若干实现, 他们是:

* NeuralNetwork. NeuralNetwork是Paddle中由各个Layer组成的一个基本计算单元。由于Layer仅仅处理在单一显卡或者CPU核上计算，将Layer的简单组合形成的NerualNetwork也仅仅支持单一显卡的计算或者单一CPU核上的计算。
* MultiGradientMachine. 在多个设备上，每一个设备构造一个NeuralNetwork，进而将输入数据切分成多个等分，进行数据并行。而多显卡多线程数据并行的逻辑，写在了MultiGradientMachine中。而对外部调用者来说，还是单线程的，调用`forward()`和`backward()`即可。
* ParallelNeuralNetwork. MultiGradientMachine处理了多显卡多线程的数据并行，而ParallelNeuralNetwork实现了模型并行，即在一个拓扑结构中，不同的Layer可以在不同的设备上执行。

下面依次简要介绍一下各个实现的细节。

## NeuralNetwork


```cpp
class NeuralNetwork {
public:
  map<std::string, ParameterPtr> parameters;
  std::vector<LayerPtr> layers;
  ...
}
```

NeuralNetwork的成员函数非常简单，主要包括了这个神经网络的参数和内部的Layer。

1. NeuralNetwork的参数要么是在NeuralNetwork中创建，或者是在更高层的类型(MultiGradientMachine)中创建，而在这里引用而成。
2. 另一点值得注意的是，`layers`是一个非常简单的数组。这个数组已经在拓扑结构中排好序构造而成。而对于有环的拓扑结构，例如RNN，Paddle使用一个特殊的layer(参考之前的Recurrent Layer)完成。

而NeuralNetwork的实现也非常简单，简要实现如下:

```cpp
void forward(std::vector<Argument> args) {
  for (auto& each: layers) {
    if (each.isDataLayer()) {
        each.putData(args);
    }
  }
  
  for (auto& layer: layers) {
    layer.forward();
  }
}

void backward(std::function<void(Parameter*)> callback) {
  for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
    auto& layer = *it;
    layer.backward(callback);
  }
}
```

## MultiGradientMachine

MultiGradientMachine是Paddle中实现多显卡多GPU训练的结构。MultiGradientMachine里面包括了多个NeuralNetwork实例，每一个实例是用一个计算单元，或使用一个CPU核心，或使用一个GPU核心。而多CPU核心与多GPU核心的计算的并行，并不完全一致。


* 对于多个CPU核心来说，他们都可以访问同一段内存。所以参数分发不是问题。对于梯度聚合来说，只需要在不同线程使用各自的梯度内存，然后再使用多线程加到主线程的梯度上即可。
* 对于多GPU来说，参数的分发和聚合比较复杂。Paddle自己实现了环形通信过程。即不同的参数具有不同的主卡。在参数分发时，由主卡向他的邻居分发参数，再由他的邻居向他邻居的邻居分发参数。在梯度聚合时，想它最远距离的邻居发送梯度给他的邻居，进而依次发送到自己。


## ParallelNerualNetwork

ParallelNerualNetwork在Paddle中实现了模型并行。模型并行，即拓扑结构的不同部分在不同的设备上运行。在Paddle实现中，对不同设备开启了一个计算线程，而在这个线程中，获得输入数据时会进行跨设备的通信。当某一个计算线程的输入数据完整后，开始这一计算线程的计算。当计算完成后，通知下一个设备的线程继续计算。

# ParameterUpdater

![alt](http://api.paddlepaddle.org/graphviz?dot=https://gist.githubusercontent.com/jacquesqiao/dcf0a5d29d8a6ffbba968553c55ccf75/raw/8067087264d103ad18480f4388601d7282800634/parameter_updater.dot)

ParameterUpdater主要用于在gradientMachine通过forward backward计算出gradient之后，调用update算法更新参数。屏蔽了是否有parameter server，cpu/gpu单卡/gpu多卡等多种情况下的差异。

ParameterUpdater由trainer创建和管理，初始化的时候将parameters作为构造参数。大体上分为两个部分，localUpdater和remoteUpdater，localUpdater负责没有parameterserver的情况下，本地直接调用ParameterOptimizer更新参数，remoteUpdater则负责管理和parameter server的通信，send/receive/update parameter。

ParameterUpdater整体接口设计。
```cpp
class ParameterUpdater {
public:
   ParameterUpdater();
   void init(const std::vector<ParameterPtr> parameters);
   void startPass();
   void finishPass();
   void startBatch();
   void update(Parameter* para);
   void finishBatch();

protected:
   std::vector<ParameterPtr> parameters_;
};
```

trainer训练逻辑
```python
        out_args = api.Arguments.createArguments(0)
        for pass_id in xrange(num_passes):
            self.__parameter_updater__.startPass()
            for batch_id, data_batch in enumerate(reader()):
                pass_type = self.__parameter_updater__.startBatch(
                    len(data_batch))
                in_args = feeder(data_batch)
                self.__prepare_parameter__(in_args)
                self.__gradient_machine__.forwardBackward(in_args, out_args,
                                                          pass_type)
                for each_param in self.__gradient_machine__.getNonStaticParameters(
                ):
                    self.__parameter_updater__.update(each_param)
                cost_sum = out_args.sum()
                cost = cost_sum / len(data_batch)
                self.__parameter_updater__.finishBatch(cost)
            self.__parameter_updater__.finishPass()
        self.__gradient_machine__.finish()
```

# ParameterOptimizer

# Trainer

TBD
