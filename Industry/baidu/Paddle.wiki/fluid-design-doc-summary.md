
## Overview design:
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/fluid.md

## Compile time:
- Program desc —> main data structures 
https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/framework/framework.proto
- Program 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/program.md
- Block 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/block.md
- var_desc 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/var_desc.md
- op_desc 
https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/framework/framework.proto#L33

## Run time:
- Tensor       
https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/framework/tensor.md
- Scope 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/scope.md
- Variable 
https://github.com/PaddlePaddle/Paddle/blob/develop/paddle/framework/variable.md
- Executor 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/executor.md
- Operator switch kernel
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/switch_kernel.md

## Other key modules
### Backward  
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/backward.md
### Optimizer 
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/optimizer.md
### Parallel design:
https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/concurrent_programming.md