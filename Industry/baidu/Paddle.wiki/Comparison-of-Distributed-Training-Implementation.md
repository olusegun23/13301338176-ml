# Comparison of Distributed Training Implementation

| Framework | Glossary | Communication | Model Parallism |
| --------- | -------- | ------------- | ------------------- |
| Caffe2    | data_parallel_model | Gloo | No |
| TensorFlow | device  | grpc | Yes |
| Paddle     | pserver/trainer | TCP | No |

## Caffe2

Caffe2 uses a "storage op" to store variables at remote side. There are two storage op implementations including: "File store", "Redis store", under `caffe2/distributed/`.These ops use "Gloo" as it's remote communication library.

Caffe2 defines it's distributed training procedures at `caffe2/python/data_parallel_model.py`

To run a training job parallel, we need to call: 
The remote storage coordinator is called "rendezvous". Sample code is at [here](https://github.com/caffe2/caffe2/blob/master/caffe2/python/examples/resnet50_trainer.py)

```python
if num_shards > 1:
        # Create rendezvous for distributed computation
        store_handler = "store_handler"
        if args.redis_host is not None:
            # Use Redis for rendezvous if Redis host is specified
            workspace.RunOperatorOnce(
                core.CreateOperator(
                    "RedisStoreHandlerCreate", [], [store_handler],
                    host=args.redis_host,
                    port=args.redis_port,
                    prefix=args.run_id,
                )
            )
        else:
            # Use filesystem for rendezvous otherwise
            workspace.RunOperatorOnce(
                core.CreateOperator(
                    "FileStoreHandlerCreate", [], [store_handler],
                    path=args.file_store_path,
                )
            )
        rendezvous = dict(
            kv_handler=store_handler,
            shard_id=shard_id,
            num_shards=num_shards,
            engine="GLOO",
            exit_nets=None)
```

And then create the `data_parallel_model`:

```python
    # Create parallelized model
    data_parallel_model.Parallelize(
        train_model,
        input_builder_fun=add_image_input,
        forward_pass_builder_fun=create_resnet50_model_ops,
        optimizer_builder_fun=add_optimizer,
        devices=gpus,
        rendezvous=rendezvous,
        optimize_gradient_memory=True,
        cpu_device=args.use_cpu,
    )
```

## TensorFlow

In TensorFlow, parameter server is described as a "device", which is returned by function `replica_device_setter` at `python/training/device_setter.py`. There are two types of devices: ps and worker. Users should put "Variables" on "ps" and put calculations on "workers":

```python
with tf.device("/job:ps/task:0"):
  weights_1 = tf.Variable(...)
  biases_1 = tf.Variable(...)

with tf.device("/job:ps/task:1"):
  weights_2 = tf.Variable(...)
  biases_2 = tf.Variable(...)

with tf.device("/job:worker/task:7"):
  input, labels = ...
  layer_1 = tf.nn.relu(tf.matmul(input, weights_1) + biases_1)
  logits = tf.nn.relu(tf.matmul(layer_1, weights_2) + biases_2)
  # ...
  train_op = ...

with tf.Session("grpc://worker7.example.com:2222") as sess:
  for _ in range(10000):
    sess.run(train_op)
```

And we define these devices by calling `tf.train.ClusterSpec`

```python
  # Create a cluster from the parameter server and worker hosts.
  cluster = tf.train.ClusterSpec({"ps": ps_hosts, "worker": worker_hosts})

  # Create and start a server for the local task.
  server = tf.train.Server(cluster,
                           job_name=FLAGS.job_name,
                           task_index=FLAGS.task_index)
```

## Paddle

To do distributed training, paddle needs to start sevaral pserver processes on different nodes, and then start trainers on another group of nodes, we tell pservers and trainers the distributed job configurations by specifing arguments to these processes, for pserver:

```bash
./pserver --use_gpu=False --port=7164 --ports_num=1 --num_of_gradient_servers=2 ...
```

For trainers:

```bash
paddle.init(use_gpu=False, port=7164, ports_num=1, is_local=False, num_of_gradient_servers=2)
```