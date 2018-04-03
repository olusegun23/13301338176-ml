### 重构的背景和目标

1. 用 PaddlePaddle API 描述DeepLearning算法，需要写很多代码。需要参考新一代的DL系统（TensorFlow Keras、PyTorch）重新设计便于使用的API。

1. PaddlePaddle代码设计繁复，用户看不懂。新加入团队的同事们也看不懂。为了提升社区活力，鼓励新成员加入贡献，提升用户对PaddlePaddle的信心，必须重写大部分代码。

1. 老一代的DeepLearning系统对计算的表示是“a sequence of layers”，新一代的表示是“a graph of operations”。需要核心代码重新设计，支持新的表示方法，提供足够的灵活度，以表达未来会研发出的新算法和新模型。

1. 团队人员输出世界一流高质量代码的一次培训。


### 计划和进展

  1. 2月：推出了新的PaddlePaddle API，使得算法开发需要写的程序行数下降了~4倍；
  1. 3月：用新 API 重写了 PaddlePaddle 的八个典型实例，验证了新API的有效性；
  1. 4月：围绕实例写了 Deep Learning 101 这本书，并且每章录制了视频公开课；
  1. 5月：开始 PaddlePaddle Cloud 的开发。开始用 IDL 内部应用验证新 API；
  1. 6月：研读TensorFlow、Caffe2、PyTorch、DyNet的源码，并且规范化；
  1. 7月：开始重构核心代码（不要只是金玉其外）。国家重点实验室机群搭建完成；
  
  1. 8月（https://github.com/PaddlePaddle/Paddle/projects/19）
     1. 基于重构后的核心代码，实现一部分常用 operators；
     1. 重新实现Python API，operators 映射成 layers —— 因为用户编程时希望逻辑表达是layers；
     1. 基于重构后的核心代码和新的Python API实现，可以实现前向神经元网络的训练和inference。用MNIST来验证；
     1. 规范开发流程：
     	1. 在编码前先写设计文档；
     	1. 编码过程中更新设计；
     	1. 开发验证之后把设计文档更新成用户文档；
     1. 规范测试流程
     	1. 规范化测试的书写；
     	1. 限制执行时间；
     	1. 确保测试覆盖率；
     	1. 自动测试grandient的计算结果；
   1. 9月中（Project：https://github.com/PaddlePaddle/Paddle/projects/20）
        1. 跑通循环神经网络：支持RNN的模型训练，选择情感分类作为第一个验证示例；
        1. 团队成员并行开发各种operators，确保可以迁移现有layer；
   1. 9月底：
        1. 重构后的Paddle核心和Paddle Cloud的分布式架构兼容；
   1. 10月：
    	1. 实现`if else then`分支operator；
        1. 迁移现有layer；
        1. 迁移现有demo/models；
        1. 结合新的应用开发新的 demo/models；
        1. 高性能加速（与INTEL&NVIDIA联合开发）；
   1. 12月初：
    	1. 完成v0.11.0版本的发布工作；