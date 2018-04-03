# Paddle 重构计划

## 目前Paddle的架构

```text
 MultiGradientMachine  <-- protobuf's ModelConfig
          +
          +
    NeuralNetwork
          +
          +
   +---+Layer+---+
   |             |
   +             +
Parameter     Argument
   +             +
   |             |
   +             +
Vector         Matrix
  +               +
  |               |
  |               |
  +--+MemHandle+--+
```

目前Paddle的架构如上图所示。有如下问题:

* 最上层与用户的接口是protobuf格式的`ModelConfig`对象。`ModelConfig`对象过于复杂冗余，`config_parser`，`trainer_config_helpers`调用过深。
* `Layer`区分看待参数(`Parameter`)与输入(`Argument`)，导致输入不能被优化。同样的代码在Paddle中会实现多次(`MulProjection`, `MulOperator`)。
* `Vector`与`Matrix`不统一。Paddle没有`Tensor`对象，导致实现数学操作没有统一的输入类型。

## 解决办法

将整个问题分为三部分解决:

* protobuf ModelConfig的清理，会使用纯C++完成拓扑格式配置，并使用C API想第三方语言暴露拓扑结构配置方法，直接用第三方语言操纵拓扑结构，抛弃Protobuf中间层。实现方法如[PR #1665](https://github.com/PaddlePaddle/Paddle/pull/1665)描述。由: 于洋负责
* 将`Layer`转换成无状态的`Function`调用。让Paddle从计算处开始，不区分`Parameter`与`Argument`。由: 何道远负责
* 将`Vector`与`Matrix`统一成`Tensor`，让Paddle的所有矩阵、向量的数学操作有统一的入口。由: 王益、冯家宜、廖刚、齐俊负责。

重构完毕后，Paddle的结构如下:

```text
      topology
         +
         |
         +
  MultiGradientMachine
        +
        |
        +
     NeuralNetwork
        +
        |
        +
  +--+Layer+-------+ Function
  |           |
  +           +
Parameter  Argument
  +            +
  |            |
  +--+Tensor+--+
```

最终，所有的Layer都是由Function构成，故一个Layer可以变成多个函数构成的子图。`MultiGradientMachine`和`NeuralNetwork`会重构成驱动`Function`在单机多线程，多显卡上计算`Engine`。

```text
           Topology
              +
              +
            Engine
          +        +
          +        +
       Function  Tensor
       
     Layer = vector<Function>.
```

## 工作计划

## Tensor实现计划

* 2017-5-8 至 2017-5-15
	* TBD
* 2017-5-15 至 2017-5-22
	* TBD 
* 2017-5-22 至 2017-5-29
	* TBD
* ...

## Function 实现计划

* 2017-5-8 至 2017-5-15
	* TBD
* 2017-5-15 至 2017-5-22
	* TBD 
* 2017-5-22 至 2017-5-29
	* TBD
* ...

## Topology 实现计划

* 2017-5-8 至 2017-5-15
	* 完成Topology重构设计文档，合入主线
	* 以`paddle::Function`为例，重构`paddle::Function`的配置部分。
		* 将`paddle::Function`从类转化为函数。
* 2017-5-15 至 2017-5-22
	* 完成Topology重构中的`Layer`部分
* 2017-5-22 至 2017-5-29
	* 暴露Paddle训练过程的C-API
	* 暴露Tensor的 C-API(期待Tensor API搞定)
* 2017-5-29 至 2017-6-5
	* 实现在C-API中定义新的拓扑结构，进行训练。