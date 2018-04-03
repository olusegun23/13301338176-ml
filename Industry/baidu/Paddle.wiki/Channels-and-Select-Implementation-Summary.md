This covers details of the recent work and implementation of two critical pieces of CSP: Channels, and the Select Operator.

# Channels redesign

The original design and implementation for Channels had the following shortcomings:
* They blocked when there were multiple senders and receivers for a channel. This is a requirement to be able to support `select`.
* Buffered and Unbuffered Channels were implemented separately, with the potential to cause inconsistency in implementations.

To solve these problems, we sought inspiration from the design of channels in Go (https://github.com/golang/go/blob/master/src/runtime/chan.go) and as summarized here: http://www.tapirgames.com/blog/golang-channel.

Key ideas that we got inspired from include:

* Use of separate send and receive queues to keep a track of channels waiting to receive or send to the queue, aside from a buffer queue that holds that data being passed.
* Direct send-receives between threads when there was nothing waiting in the buffer.
* Unified implementation for both buffered and unbuffered channels.
* Scenarios in which to panic and block.

The redesign also changes the contents of the buffer queue to be a `QueueMessage` that can hold data of any template type and allows blocking using a conditional variable.

# Select design and implementation

Select has been modeled after Go's `select` syntax. Similar to what has been proposed in [the original design doc](https://github.com/PaddlePaddle/Paddle/blob/develop/doc/design/csp.md), the Python API into the operators looks like:

```
with fluid.Select() as select:
    with select.case(fluid.channel_send, ch1, input_value):
        # Execute something.
        ...

    with select.case(fluid.channel_recv, ch2, result2):
        # Execute something with result2.
        pass

    with select.default():
        pass
```

Note that the implementation does NOT actually use the `fluid.channel_send` or `fluid.channel_recv` ops. Instead, highlights of its implementation are as follows:

* There is a `select` op, and for each possible case inside of the select, a `conditional_block` op is used. All the `conditional_block`s are encapsulated in a separate block, which we can call the "cases block".
* The channel variables, values to be sent or received, whether the cases are `send`s or `recv`s are known to the `select` op by traversing the `conditional_block`s inside the cases block, and accessing their attributes where these values are set.
* The execution of the `select` op and its communication protocol with Channels is inspired by Go's implementation (https://github.com/golang/go/blob/master/src/runtime/select.go) which has been summarized well here: http://www.tapirgames.com/blog/golang-concurrent-select-implementation
* The `select` op waits until it can resolve which case it needs to be executed.
* It is the responsibility of `select` op to set a variable that shall be used by each of the `conditional_block`s with the help of a `equals` op to determine whether it should execute the statements within its block.

Here is the set of BlockDescs it creates in the ProgramDesc. Each box here is a block.

![Blocks created by Select](https://raw.githubusercontent.com/bobateadev/images/master/Select%20ProgramDesc.jpg)

The last of the conditional blocks here is for the `default` case.