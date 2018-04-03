# Regularization Survey

On surveying existing frameworks like Tensorflow, PyTorch, Caffe, etc, it can be seen that there are 2 common approaches of doing regularization:

1. Making regularization a part of the optimizer using an attribute like `weight_decay` that is used to control the scale of the L2 Penalty. This approach is used in PyTorch as follows:
  ```python
  opt =  torch.optim.SGD(params, lr=0.2, weight_decay=0.2)
  ```
    At every optimization step, this code will add the gradient of the L2 Norm of the params to the gradient of the params with respect to the loss function. This can seen in the following code snippet:
  ```python
  if weight_decay != 0:
      d_p.add_(weight_decay, p.data)
  ```
    This is a very restrictive way of doing regularization and does not give the users enough flexibility. 

**Advantages**:
  -  It is easy to implement for us.
  -  Faster execution of backward. However, it can be done manually by advanced users too.

**Disadvantages**:
  - Not flexible for other regularizations such as L1/L0 regularization.
  - Does not allow for different regularization coefficient for different parameters. For example, in most models, only the weight matrices are regularized and the bias vectors are unregularized.
  - Tightly coupled optimizer and regularization implementation. 
  - Without forward op for regularization, it is not possible to plot a graph of overall loss vs iteration


2. Adding regularization ops to the graph through Python API. This approach is used by Tensorflow, Theano and Lasagne. Using this approach, we manually add regularization ops to the graph and then add the regularization loss to the final loss function before sending them to the optimizer.

**Advantages**:
  - Allows for greater flexibility to the users of Paddle. Using this approach, the users can put different regularization to different parameters and also choose parameters that are not a part of regularization.
  - Makes it easy for the users to customize and extend the framework. 

**Disadvantages**:
  - Implementation requires comprehensive design and time. 

Now, let us look at each framework in a little bit more detail. 

### Tensorflow

In Tensorflow, regularization is done by adding regularization ops to the graph. Some points to note about tensorflow implementation are as follows:
  - Tensorflow provides L1 and L2 regularization through the [`tf.contrib`](https://www.tensorflow.org/versions/r0.12/api_docs/python/contrib.layers/regularizers) package. These ops are added to the graph and are executed in both forward and backward pass. 
  - In Tensorflow all the regularization losses are added to the [`tf.GraphKeys.REGULARIZATION_LOSSES`](https://www.tensorflow.org/api_docs/python/tf/GraphKeys). This is like a global variable where the regularization losses are accumulated. It is the responsibility of the user to add these losses to the classification loss before optimization. 
  - In Python code of Tensorflow, regularization is specified while creating layers through `tf.contrib.layers`. An example can be found [here](https://www.tensorflow.org/versions/r0.12/api_docs/python/contrib.layers/higher_level_ops_for_building_neural_network_layers_#fully_connected).

### PyTorch

PyTorch only supports L2 Regularization through the weight decay attribute in its optimizers. 
  ```python
  opt =  torch.optim.SGD(params, lr=0.2, weight_decay=0.2)
  ```
At every optimization step, this code will add the gradient of the L2 Norm of the params to the gradient of the params with respect to the loss function. This can seen in the following code snippet:
  ```python
    if weight_decay != 0:
      d_p.add_(weight_decay, p.data)
  ```
An important point to note is that PyTorch supports L2 regularization in **all** its optimizers through the weight decay. 

### Caffe 

Caffe supports both L2 and L1 regularization through the weight decay approach. However, it supports regularization for the [SGD Solver](http://caffe.berkeleyvision.org/tutorial/solver.html). The code for the weight decay can be found [here](https://github.com/BVLC/caffe/blob/master/src/caffe/solvers/sgd_solver.cpp#L145)

### Caffe2 

Caffe2 is one of the most unique frameworks in terms of regularization. Caffe2 supports both the above discussed methods for regularization.

1. Caffe2 supports L2 regularization by weight decay in its SGD optimizers. This is similar to the implementation in PyTorch and Caffe. This can be seen in the following [operator](https://github.com/caffe2/caffe2/blob/master/caffe2/sgd/fp32_momentum_sgd_op.h#L51) and also in the following [Python API](https://github.com/caffe2/caffe2/blob/master/caffe2/python/helpers/train.py#L73).

2. In addition to weight decay, Caffe2 also supports regularization by adding operators to the forward pass that add the regularization loss to the overall loss. This implementation can be found in the following Python [module](https://github.com/caffe2/caffe2/blob/master/caffe2/python/regularizer.py)