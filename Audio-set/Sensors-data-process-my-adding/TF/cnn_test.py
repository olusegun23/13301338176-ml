# -*- coding: utf-8 -*-
"""
Warm-up: numpy
--------------


"""
import numpy as np

import tensorflow as tf
#case 2
input = tf.Variable(tf.random_normal([1,3,3,5]))
filter = tf.Variable(tf.random_normal([1,1,5,1]))

op2 = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='VALID')
#case 3
input = tf.Variable(tf.random_normal([1,3,3,5]))
filter = tf.Variable(tf.random_normal([3,3,5,1]))

op3 = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='VALID')
#case 4
input = tf.Variable(tf.random_normal([1,5,5,5]))
filter = tf.Variable(tf.random_normal([3,3,5,1]))

op4 = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='VALID')
#case 5
input = tf.Variable(tf.random_normal([1,5,5,5]))
filter = tf.Variable(tf.random_normal([3,3,5,1]))

op5 = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='SAME')
#case 6
input = tf.Variable(tf.random_normal([1,5,5,5]))
filter = tf.Variable(tf.random_normal([3,3,5,7]))

op6 = tf.nn.conv2d(input, filter, strides=[1, 1, 1, 1], padding='SAME')
#case 7
input = tf.Variable(tf.random_normal([1,5,5,5]))
filter = tf.Variable(tf.random_normal([3,3,5,7]))

op7 = tf.nn.conv2d(input, filter, strides=[1, 2, 2, 1], padding='SAME')
#case 8
input = tf.Variable(tf.random_normal([10,5,5,5]))
filter = tf.Variable(tf.random_normal([3,3,5,7]))

op8 = tf.nn.conv2d(input, filter, strides=[1, 2, 2, 1], padding='SAME')

init = tf.initialize_all_variables()
with tf.Session() as sess:
    sess.run(init)
    print("case 2")
    print(sess.run(op2))
    print("case 3")
    print(sess.run(op3))
    print("case 4")
    print(sess.run(op4))
    print("case 5")
    print(sess.run(op5))
    print("case 6")
    print(sess.run(op6))
    print("case 7")
    print(sess.run(op7))
    print("case 8")
    print(sess.run(op8))
















img = tf.constant(value=[[[[1],[2],[3],[4]],[[1],[2],[3],[4]],[[1],[2],[3],[4]],[[1],[2],[3],[4]]]],dtype=tf.float32)
img = tf.concat(values=[img,img],axis=3)
filter = tf.constant(value=1, shape=[3,3,2,5], dtype=tf.float32)
out_img1 = tf.nn.atrous_conv2d(value=img, filters=filter, rate=1, padding='SAME')
out_img2 = tf.nn.atrous_conv2d(value=img, filters=filter, rate=1, padding='VALID')
out_img3 = tf.nn.atrous_conv2d(value=img, filters=filter, rate=2, padding='SAME')

#error
#out_img4 = tf.nn.atrous_conv2d(value=img, filters=filter, rate=2, padding='VALID')

with tf.Session() as sess:
    print 'rate=1, SAME mode result:'
    print(sess.run(out_img1))

    print 'rate=1, VALID mode result:'
    print(sess.run(out_img2))

    print 'rate=2, SAME mode result:'
    print(sess.run(out_img3))

    # error
    #print 'rate=2, VALID mode result:'
    #print(sess.run(out_img4))








img1 = tf.constant(value=[[[[1],[2],[3],[4]],[[1],[2],[3],[4]],[[1],[2],[3],[4]],[[1],[2],[3],[4]]]],dtype=tf.float32)
img2 = tf.constant(value=[[[[1],[1],[1],[1]],[[1],[1],[1],[1]],[[1],[1],[1],[1]],[[1],[1],[1],[1]]]],dtype=tf.float32)
img = tf.concat(values=[img1,img2],axis=3)
filter1 = tf.constant(value=0, shape=[3,3,1,1],dtype=tf.float32)
filter2 = tf.constant(value=1, shape=[3,3,1,1],dtype=tf.float32)
filter3 = tf.constant(value=2, shape=[3,3,1,1],dtype=tf.float32)
filter4 = tf.constant(value=3, shape=[3,3,1,1],dtype=tf.float32)
filter_out1 = tf.concat(values=[filter1,filter2],axis=2)
filter_out2 = tf.concat(values=[filter3,filter4],axis=2)
filter = tf.concat(values=[filter_out1,filter_out2],axis=3)

out_img = tf.nn.depthwise_conv2d(input=img, filter=filter, strides=[1,1,1,1], rate=[1,1], padding='VALID')

print ("==========================")
print (out_img)





exit(0)