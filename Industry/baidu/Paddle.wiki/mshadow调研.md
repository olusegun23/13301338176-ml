mshadow除了Tensor的表示以外，还包含了计算的实现。为了保证计算高效，mshadow的整体设计参考了惰性计算（lazy operation）的原则。关于lazy operation的详细信息可以参考[这里](https://github.com/dmlc/mshadow/blob/master/guide/exp-template/README.md)。

## Exp
Exp是mshaow中最重要的部分，用于表示一个表达式，例如`a+b`。任何可以放在`=`两边的内容都可以被认为是`Exp`。显然，单一的一个tensor也是一个Exp。`Exp`的基类定义如下：

```c++
template<typename SubType, typename DType, int exp_type>
struct Exp {
 public:
  /*! \return  subtype instance of current class */
  inline const SubType& self(void) const {
    return *static_cast<const SubType*>(this);
  }
  /*! \return reference of subtype instance of current class */
  inline SubType* ptrself(void) {
    return static_cast<SubType*>(this);
  }
};
```

`Exp`本身并不会被直接使用，使用的是其经过[CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)派生后的基类。经过特化和派生，`Exp`的各种子类被用于表示各种各样的表达式。模板参数的意义如下：

- `SubType`表示经过派生后的具体类型，例如`Tensor`
- `Dtype`表示数据的类型
- `exp_type`表示表达式自身的的类型，分为`kRValue`、`kMapper`、`kChainer`和`kComplex`四种。
	- `kRValue`：已经存在、可直接使用的数据。
	- `kMapper`：两个tensor之间element-wise的操作。
	- `kChainer`：由上面两种表达式组成的复杂表达式。
	- `kComplex`：其他表达式。例如点积操作。

`Exp`的主要派生类有：

- `TernaryMapExp`：三元操作
- `BinaryMapExp`：两元操作
- `UnaryMapExp`：一元操作
- `RValueExp`已经存在、可以直接使用的数据，其`exp_type`一定为`kRValue`。

另外mshadow也还提供了一些专门处理特定操作的`Exp`，例如：

- `ScalarExp`表示一个标量
- `TypecastExp`数据类型转换表达式
- `TransposeExp`转置表达式
- `DotExp`点积操作

其中`RValueExp`是`Tensor`、`Tensor1D`等具体数据类型的基类，同时它也是唯一一个可以放在等号左边的`Exp`。mshadow通过重载`RValueExp`的`+=`、`-=`等和赋值有关的operator实现了惰性计算（lazy operation）。

## Shape

`Shape`本质上就是一个整数数组，用来表示`Tensor`的维度。

## Tensor
mshadow中，`RValueExp`派生出`TRValue`，`TRvalue`又进一步派生出`Tensor`，它主要有如下模板参数：
	
- `typename Device`：GPU / CPU
- `int dimension`：维度
- `typename Dtype`：数据类型

`Tensor`的主要成员变量有：

- `Dtype *dptr_`指向数据的指针
- `Shape<dimension> shape_`维度
- `int stride_`：`Tensor`的最底层维度上相邻元素在内存上的距离。如果元素紧密排列，则`stride_ = 1`

`*dptr_`指向的内存的管理不由`Tensor`负责。因此`Tensor`的新建和销毁一般通过全局函数`NewTensor`和`FreeSpace`进行，这两个函数在构造或者析构`Tensor`之前会先进行内存的申请和销毁。

除了必要的构造函数等，Tensor本身还提供了其他一些成员函数，主要有：

- `MemSize`返回某个维度上相邻元素在内存上的距离  
- `FlatTo1D`/`FlatTo2D`将Tensor在逻辑上转化为一维 / 二维  
- `operator[]`返回某个idx上维度为d-1的`Tensor`（浅拷贝）  
- `Slice`类似于Python的切片操作  

Tensor本身没有包含任何计算的定义。

## Plan
一个完整的计算表达式一般包含两个部分：被计算对象和计算操作，其中计算对象在`Exp`及其派生类中定义，计算操作则在`Plan`中定义。

`Plan`的定义为：

```c++
template<typename ExpType, typename DType>
class Plan {
 public:
  /*!
   * \brief evaluate the expression at index [y][x]
   *  to be implemented by SubType, for RValue, the return type will be DType &
   */
  MSHADOW_XINLINE DType Eval(index_t y, index_t x) const;
};
```
通过用`ExpType`作为模板参数进行特化，`Plan`与具体的`Exp`一一绑定，每一个`Exp`的派生类都需要定义自己的`Plan`。从逻辑上来说，任何表达式执行完成后，一定会得到一个tensor，对表达式计算操作的定义，其实就等价于定义结果tensor中每一个元素生成的方法。这个定义在`Plan`中由`Eval`函数完成。

例如，当表达式是一个简单的tensor时候的特化：

```c++
template <typename Device, int dim, typename DType>
class Plan<Tensor<Device, dim, DType>, DType> {
 public:
  explicit Plan(const Tensor<Device, dim, DType> &t)
      : dptr_(t.dptr_), stride_(t.stride_) {}
  // for RValue, the return type should be reference
  MSHADOW_XINLINE DType &REval(index_t y, index_t x) {
    return dptr_[y * stride_ + x];
  }
  // const evaluation
  MSHADOW_XINLINE const DType &Eval(index_t y, index_t x) const {
    return dptr_[y * stride_ + x];
  }

 private:
  DType  *dptr_;
  index_t stride_;
};
```

`Eval`函数所做的就是直接取出对应位置上的值。

需要特别指出，`Eval`函数只有两个参数输入，因此只能处理结果为二维tensor的表达式。事实上，为了更简单地利用多线程加速，mshadow会在使用CPU时将任何表达式的结果tensor在逻辑上转化为二维，再交由Plan处理。

## ExpEngine

mshadow中定义了名为`ExpEngine`的struct，用来执行一个等式，这里的等式不一定要有等号，也可以是`+=`、`-=`、`*=`、`/=`这些赋值操作。上文已经提到，等式的左边一定是一个`RValueExp`，mshadow重载了`RValueExp`的赋值operator，在内部调用`ExpEngine`来完成等式右边表达式的计算和对左边表达式的赋值。

`ExpEngine`的核心是`Eval`成员函数。根据**赋值操作**的不同（=，+=，-=，*=，/=）和**右侧表达式类型**的不同（kMapper，kChainer，kRValue，kComplex），`Eval`函数有多种类型的特化：

```c++
template<typename SV, typename RV, typename DType>
struct ExpEngine {
  template<typename E>
  inline static void Eval(RV *dst,
                          const Exp<E, DType, type::kMapper> &exp) {
    MapExp<SV>(dst, exp);
  }
  template<typename E>
  inline static void Eval(RV *dst,
                          const Exp<E, DType, type::kChainer> &exp) {
    MapExp<SV>(dst, exp);
  }
  template<typename E>
  inline static void Eval(RV *dst,
                          const Exp<E, DType, type::kRValue> &exp) {
    MapExp<SV>(dst, exp);
  }
  template<typename E>
  inline static void Eval(RV *dst,
                          const Exp<E, DType, type::kComplex> &exp) {
    ExpComplexEngine<SV, RV, E, DType>::Eval(dst->ptrself(), exp.self());
  }
};
```
其中的模板参数`SV`表示具体的赋值操作，可以看到`Eval`函数会根据具体情况进一步调用`MapExp`或者将需要执行的等式转交给`ExpComplexEngine`执行。在CPU环境中，`MapExp`函数在经过一系列的检查后最终调用全局函数`MapPlan`：

```c++
template<typename Saver, typename R, int dim,
         typename DType, typename E>
inline void MapPlan(TRValue<R, cpu, dim, DType> *dst,
                    const expr::Plan<E, DType> &plan) {
  Shape<2> shape = expr::ShapeCheck<dim, R>::Check(dst->self()).FlatTo2D();
  expr::Plan<R, DType> dplan = expr::MakePlan(dst->self());
#if (MSHADOW_USE_CUDA == 0)
  #pragma omp parallel for
#endif
  // temp remove openmp, as default setting throttles CPU
  for (openmp_index_t y = 0; y < shape[0]; ++y) {
    for (index_t x = 0; x < shape[1]; ++x) {
      // trust your compiler! -_- they will optimize it
      Saver::template Save<DType>(dplan.REval(y, x), plan.Eval(y, x));
    }
  }
}
```

`MapPlan`函数将左边的Tensor和右边表达式的计算结果在逻辑上平摊至二维以便于多线程并行。

`MapPlan`的本质在于建立等式两边表达式的计算结果在元素上的映射关系。例如在上面的例子中：

```c++
Saver::template Save<DType>(dplan.REval(y, x), plan.Eval(y, x));
```

其中，`plan`是等式右边表达式的`Plan`，通过`Eval`获得了其计算结果在[y,x]位置上的元素；`dplan`是等式左边表达式（一般就是一个tensor）的`Plan`，`REval`获得了该tensor在[y,x]位置上元素的引用。`Save`函数进行两个元素之间的赋值操作，这个操作具体是`+=`还是`-=`，或者是别的什么操作，则由`Save`的特化来确定。

## 各个概念相互之间的关系
可以通过一个简单的例子来说明上面出现过的各个概念相互之间的关系：

```
A += B + C
```

其中`A``B``C`都是tensor，`+`为逐元素相加。

在这个例子中，`A`作为等式左边的接受计算结果的对象，是一个`RValueExp`，`B + C`则是一个一般类型的`Exp`。

`A`和`B + C`都有用自己的类型特化产生的`Plan`，`Plan`中有`Eval`函数，它定义了表达式执行后的结果tensor中每一个元素的产生规则。例如，`A + B`这个`Exp`特化产生的`Plan<Exp>`中的`Eval`，就会定义`res[i,j] = B[i,j] + C[i,j]`这样的规则。

`MapPlan`函数则定义了等式左右两边元素之间的赋值关系和具体的赋值操作。例如本例子中为对应元素之间的累加赋值。

`A`作为一个`RValueExp`，它的`operator+=`已经被重载，会在内部调用`ExpEngine`中的`Eval`函数，执行上述的计算操作。


## TensorContainer
对Tesnor的进一步派生，自带内存的申请和释放，并以STL的风格管理内存，因此可以进行resize。

## TBlob
用于暂存任意维度、设备和数据类型的Tensor。TBlob本身不提供任何算法，仅仅用于暂时持有一个Tensor。Tensor本身所具有的模板参数，在TBlob中成为变量保存。