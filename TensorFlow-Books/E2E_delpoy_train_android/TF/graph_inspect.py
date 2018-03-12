import tensorflow as tf


graph_def = tf.GraphDef()
dir(graph_def)
with open("optimized_tfdroid.pb", "rb") as f:
     graph_def.ParseFromString(f.read())
for node in graph_def.node:
    print(node)
 
for node in graph_def.node:
    print(node.name)
