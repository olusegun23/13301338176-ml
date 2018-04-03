```python
from __future__ import print_function
import numpy as np
import paddle.v2 as paddle
import paddle.v2.fluid as fluid
import sys
# just define to_lodtensor function for inner call
def to_lodtensor(data, place):
    seq_lens = [len(seq) for seq in data]
    cur_len = 0
    lod = [cur_len]
    for l in seq_lens:
        cur_len += l
        lod.append(cur_len)
    flattened_data = np.concatenate(data, axis=0).astype("int64")
    flattened_data = flattened_data.reshape([len(flattened_data), 1])
    res = fluid.LoDTensor()
    res.set(flattened_data, place)
    res.set_lod([lod])
    return res
 
def load_vocab(filename):
    vocab = {}
    with open(filename) as f:
        wid = 0
        for line in f:
            vocab[line.strip()] = wid
            wid += 1
    return vocab
# load word dict with paddle inner function
word_dict = load_vocab(sys.argv[1])
word_dict["<unk>"] = len(word_dict)
print(len(word_dict))
# vocabulary size
dict_dim = len(word_dict)
 
# embedding dim
emb_dim = 128
 
# hidden dim
hid_dim = 128
 
# hidden dim2
hid_dim2 = 96
 
# class num
class_dim = 2
 
# input text data
data = fluid.layers.data(
    name="words", shape=[1], dtype="int64", lod_level=1)
 
# label data
label = fluid.layers.data(name="label", shape=[1], dtype="int64")
 
# embedding
emb = fluid.layers.embedding(input=data, size=[dict_dim, emb_dim])
 
# sequence conv with window size = 3
win_size = 3
conv_3 = fluid.nets.sequence_conv_pool(
    input=emb,
    num_filters=hid_dim,
    filter_size=win_size,
    act="tanh",
    pool_type="max")
 
# fc layer after conv
fc_1 = fluid.layers.fc(input=[conv_3],
                       size=hid_dim2)
 
# probability of each class
prediction = fluid.layers.fc(input=[fc_1],
                             size=class_dim,
                             act="softmax")
 
# cross entropy loss
cost = fluid.layers.cross_entropy(input=prediction, label=label)
 
# mean loss
avg_cost = fluid.layers.mean(x=cost)
 
# SGD optimizer
sgd_optimizer = fluid.optimizer.SGD(learning_rate=0.01)
sgd_optimizer.minimize(avg_cost)
 
# accuracy metric
accuracy = fluid.evaluator.Accuracy(input=prediction, label=label)
inference_program = fluid.default_main_program().clone()
with fluid.program_guard(inference_program):
    test_target = accuracy.metrics + accuracy.states
    inference_program = fluid.io.get_inference_program(test_target)
 
# train data set
BATCH_SIZE = 4
train_reader = paddle.batch(
    paddle.reader.shuffle(
        paddle.dataset.imdb.train(word_dict), buf_size=5000),
    batch_size=BATCH_SIZE)
test_reader = paddle.batch(
    paddle.reader.shuffle(
        paddle.dataset.imdb.test(word_dict), buf_size=5000),
    batch_size=BATCH_SIZE)
 
# train in cpu
place = fluid.CPUPlace()
def test(exe):
    accuracy.reset(exe)
    for batch_id, data in enumerate(test_reader()):
        input_seq = to_lodtensor(map(lambda x:x[0], data), place)
        y_data = np.array(map(lambda x: x[1], data)).astype("int64")
        y_data = y_data.reshape([-1, 1])
        acc = exe.run(inference_program,
                      feed={"words": input_seq,
                            "label": y_data})
    return accuracy.eval(exe)
 
# just like session in tensorflow
exe = fluid.Executor(place)
 
# like placeholder
feeder = fluid.DataFeeder(feed_list=[data, label], place=place)
 
# not sure what's going on here
exe.run(fluid.default_startup_program())
 
# loop for 30 epochs, print acc ever 1000 batch
PASS_NUM = 30
for pass_id in xrange(PASS_NUM):
    accuracy.reset(exe)
    for data in train_reader():
        cost_val, acc_val = exe.run(fluid.default_main_program(),
                                    feed=feeder.feed(data),
                                    fetch_list=[avg_cost, accuracy.metrics[0]])
        pass_acc = accuracy.eval(exe)
    # save model every epoch
    pass_test_acc = test(exe)
    print("test_acc: %f" % pass_test_acc)
```