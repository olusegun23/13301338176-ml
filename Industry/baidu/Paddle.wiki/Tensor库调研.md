## Tensor


### Tensor库介绍

Tensor的主要功能有三个：

- 表示Tensor
- 实现数学运算的全局函数
- 实现element-wise计算与lazy求值

以下将详细介绍着三个部分的内容。

#### 表示Tensor 

Tensor中包含一个指针，可以通过这个指针操作CPU内存或GPU显存。Tensor只对数据进行描述，包括维度，每个维度上的stride，数据类型等；并不负责该块数据的分配与释放。

> [yi] Then who is responsible for memory allocaiton?  Let'us don't leave broken logic in this article.

我们可以使用模板类来去描述一个Tensor：

```
template <typename Device, typename T, int D>
class Tensor {
  Dim<D> stride_;
  Dim<D> dim_;
  T* data_;
};
```
其中Device表示数据在CPU或GPU上；T表示内存中数据类型；D表示维度信息。

> [yi] I noticed that [mshadow](https://github.com/dmlc/mshadow/blob/master/mshadow/tensor.h#L400) defines devices as a template parameter; however, I doubt this is right. We know that a class template is not a type, its specialization is.  For example, above `Tensor` is not a type/class, but `Tensor<gpu, 1>` is a type/class.  I'd thought that a tensor is conceptually a *multi-dimensional matrix*,  other than a *multi-dimensional matrix resides on the second GPU*.  I'd prefer a `Place` function parameter for the constructor of class Tensor, i.e., the [Majel way](https://github.svail.baidu.com/baidu-research/majel/blob/master/include/majel/array.h#L35).

#### 实现数学运算的全局函数

所有对Tensor的操作都是全局函数，全局函数有两种形式，分别是无返回值和有返回值的。

> [yi] How comes this statement "所有对Tensor的操作都是全局函数"? Obviously, PaddlePaddle's math operations are not global functions.

```
template<typename Device, typename T, int D>
void add(Tensor<Device, T, D>& in1, Tensor<Device, T, D>& in2, Tensor<Device, T, D>& out);

template<typename Device, typename T, int D>
Tensor<Device, T, D> add(Tensor<Device, T, D>& in1, Tensor<Device, T, D>& in2);

```
> I'd suggest that we follow the [Google C++ Style](https://google.github.io/styleguide/cppguide.html#Function_Parameter_Ordering)
> 
> 1. Input parameters are usually values or const references, while output and input/output parameters will be pointers to non-const. 
> 1. put all input-only parameters before any output parameters
> 
> So above definition should be `void add(const Tensor<Device, T, D>& in1, const Tensor<Device, T, D>& in2, Tensor<Device, T, D>* out);`

在实现全局函数的时候，我们可以进一步的把操作分类为UaryOperator, BinaryOperator, TenaryOperator等，add，exp，log之类的操作都只是上述Operator的一个特例。这样我们仅仅需要分别对这三种操作实现CPU/GPU版本的计算，从而避免了重复书写CUDA kernel等。

> [yi] I understand that fact that `add` is a BinaryOperator. But why this fact can save us from writing "duplicated CUDA kernels"?  Indeed, what do you mean "duplicated CUDA kernels"?

实现的全局函数可以通过注册机制，把该全局函数注册为对应的function或者Op。

> [yi] Why would we need to register global math functions?  If we are going to call one, we can just call it.  Registration is a program pattern that calls a function by its name (a string).  I think that a *Layer* should be registered because when users configure an NN, they refer to layers by their names.  But I cannot imagine that users would refer to global math functions, such fine-grained concepts, by their names in their NN configurations.

#### 实现element-wise计算与lazy求值

在书写Tensor的计算时，我们希望提供与Matlab类似写法:

```
A = B + C;
A = D * 2 + A - C;
```

这种Tensor的计算公式会在Optimization计算时，以及实现较复杂的Op的forward/backward时，会经常遇到。
Element-wise的操作可以使得计算的书写过程体验较好，避免了手动写for循环依次计算。

> [yi] Tensor addition and multiplication are common operations. Why we'd mention them specifically here and even gave them a name "element-wise"?

同时，我们也希望提高计算的效率。在这里，使用expression templates的技巧，实现lazy求值。即所有的运算都会表示为expression，我们可以选择在```=```时进行求值，从而避免了单纯使用运算符重载带来的中间变量的开销。

另一方面，我们还可以对多个表达式进行lazy求值:

```
auto expr1 = A.lazyAssign(B + C);
auto expr2 = A.lazyAssign(D * 2 + Expr<log>(A) - Expr<sqrt>(C));
AssignEvaluate(expr1, expr2);

```

从而把多个表达式合并在一个CUDA kernel里面，减少了CUDA kernel启动次数，提高效率。


> [yi] This is an interesting idea. I noticed that mshadow is doing this.  But it seems a higher level concept than tensor and math operations, which should focus on implementing optimizing elementary operations using SIMD and CUDA instructions.  I could be wrong. Would like to learn more about this idea. 

### Tensor库调研
#### Paddle

Paddle的math部分的代码介绍可以参考[wiki](https://github.com/PaddlePaddle/Paddle/wiki/Paddle%E7%9B%AE%E5%89%8D%E7%9A%84%E5%AE%9E%E7%8E%B0%E7%9A%84%E5%90%84%E4%B8%AA%E6%A8%A1%E5%9D%97%E7%BB%86%E8%8A%82#math)。

Paddle矩阵库因为历史原因，codebase较为复杂，存在的主要问题有以下几个：

- Tensor描述力度不够，关系不够清晰，目前只能描述一维的Vector以及二维的Matrix，支持的数据类型有限
- 数学计算的函数被定义为Matrix的成员方法，而不是全局函数
- 矩阵库中存在大量的描述Op计算的成员方法，这些应该被剥离出来


Paddle的math库的主要优点：

- 对sparse计算的良好支持
- 对element-wise计算，以及lazy求值的良好支持

> [yi] As a reader, I'd expected that above Wiki links can provide us more information about the two pros, but I cannot even find the word "lazy" in that Wiki page.  I also searched "sparse" in that page.  The results are just some class names including the word "sparse".  When we write a tech doc, please make sure that we present the facts/evidence that can support our opinions/statements/conclusions.

#### Majel

Majel是Baidu AI Lab的作品。Majel不仅仅是一个Tensor库，还包括庞大的scheduler，memory allocation模块等。我们可以从中借鉴Tensor的设计部分。

Majel的存在的主要问题有以下几个：

- 仅支持GPU版本，不支持纯CPU版本
- 抽象层次较多，包括DArray, Array, ArrayView三层抽象(可能不需要这么多)
- 支持element-wise的计算，以及lazy求值，但是书写较为复杂，不方便

下面举例进行说明：

include/majel/dops.h中定义了对外的操作接口 可以针对任意类型的DArray

```
void copy(const DArray src, DArray dst);
```


src/dynamic_ops/目录中定义了具体的实现

这一层判断大小不等的情况，直接报错
支持数据类型不同

```
void copy(const DArray src, DArray dst) {
    CopyVisitor cv;
    boost::apply_visitor(cv, src, dst);
}
```

这个具体的实现实际上调用的是include/majel/ops.h中的接口

这一层中会判断size和place的相符性，然后进行调度



```
template<typename Src, typename Dst, int D>
void copy(const Array<Src, D> src, Array<Dst, D> dst) {
......
scheduler::schedule([=](const scheduler::LaunchInfo& li) {
                detail::copy(make_view(src), make_view(dst), li);
            }, {src}, {dst}, std::move(metrics));

}
```

这个接口实际上是在 include/majel/detail/ops.h中实现的



```
template<typename ViewIn, typename ViewOut>
void copy(const ViewIn in, ViewOut out, const scheduler::LaunchInfo& li) {
    using Src = typename ViewIn::value_type;
    using Dst = typename ViewOut::value_type;
    UnaryOpVisitor<convert<Dst>, ViewIn, ViewOut>
            v(convert<Dst>(), in, out);
    boost::apply_visitor(v, li);
}
```


Majel的优点如下：

- 灵活的Tensor描述，可以支持任意维度，任意维度上的stride(借助模板元编程技巧)
- 迭代器访问的支持(抽象在ArrayView层面)，可以做到非常细粒度的数据访问
- 丰富的数学操作的支持，包括一些高阶操作，比如reduce，gather，bsxfun等(像matlab一样的体验)


#### Mxnet(mshadow)
Mxnet使用的Tensor库是mshadow。mshadow只负责描述Tensor，以及Tensor相关的element-wise计算与lazy求值。对于全局函数部分，有一部分放在src/operator/tensor目录下，可以通过注册机制，分别实现symbolic与imperative的计算。

mshadow存在的主要问题：

- Tensor的表示不支持任意维度上的stride
- lazy求值只应用在单条表达式上，不能把多个表达式合并做进一步优化。

mshadow的优点如下：

- mshadow整体是一个头文件，引用该库非常方便
- 较丰富数学计算的支持，包括reduce，slice，broadcast等
- 对element-wise计算的良好支持

关于mshadow设计上的更多信息，可以参考[这里](https://github.com/PaddlePaddle/Paddle/wiki/mshadow%E8%B0%83%E7%A0%94)

#### Caffe2

Caffe2没有独立的Tensor库，代码较为分散。在计算时，直接操作裸指针。

主要问题：

- 仅能表示连续内存，不支持stride
- CPU的element-wise的计算通过eigen实现(也存在部分代码手写for循环实现)；GPU的代码需要手动写for循环实现的kernel
- 提供的math计算函数较为简单；有一些高阶操作直接写在operator里面，只支持symbolic的使用

优点：

- 代码组织良好，CPU和GPU代码分开，编译依赖清晰

### 设计方案
对比不同的框架设计之后，我们需要做出决策，选择设计方案。有几个共同的问题需要考虑与注意：

- 如何清晰优雅的组织CPU与GPU代码
- Tensor是否应该保存设备id信息，以及设备信息是成员变量(运行期检查)，还是模板参数(编译器检查)
- Tensor不负责内存管理，stream，cublasHandle的管理等，有另外一个模块负责
- stride是否需要支持，支持到什么程度


具体的设计方案有三种：

#### 引入开源

可以考虑引入mxnet使用的mshadow，tensorflow使用的eigen

优点：

- 工作量会少很多

所需工作与潜在问题：

- 需要做一定的适配工作，包括更灵活的Tensor，支持lazy求值的进一步优化
- 引入第三方，可能会降低框架的整体把控能力



#### 引入Majel

优点：

- Tensor表示非常灵活
- 支持灵活高效的迭代器访问

所需工作与潜在问题：

- 需要精简Majel的抽象层次
- 重载操作符，使得书写element-wise操作更加简便


#### 重构Paddle现有矩阵库


优点：

- 可以逐步替换，目前只提供Tensor的描述
- element-wise计算与lazy求值的完备支持

所需工作与潜在问题：

- 增加一个Tensor的表示，兼容现有Matrix
- 清理现有代码，数学计算部分，由Matrix的成员方法变为全局函数



