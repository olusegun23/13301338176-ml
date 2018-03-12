# -*- encoding: utf-8 -*-

# converting a unknown formatting file in utf-8




# Preparing a TF model for usage in Android
# By Omid Alemi - Jan 2017
# Works with TF r1.0

import sys
import tensorflow as tf
from tensorflow.python.tools import freeze_graph
from tensorflow.python.tools import optimize_for_inference_lib


MODEL_NAME = 'tfdroid'

# Freeze the graph

input_graph_path = MODEL_NAME+'.pbtxt'
checkpoint_path = './'+MODEL_NAME+'.ckpt'
input_saver_def_path = ""
input_binary = False
output_node_names = "O"
restore_op_name = "save/restore_all"
filename_tensor_name = "save/Const:0"
output_frozen_graph_name = 'frozen_'+MODEL_NAME+'.pb'
output_optimized_graph_name = 'optimized_'+MODEL_NAME+'.pb'
clear_devices = True


"""

https://www.tensorflow.org/mobile/prepare_models
To handle the conversion, you’ll need the freeze_graph.py script, that’s held in tensorflow/python/tools/freeze_graph.py. You’ll run it like this:
bazel build tensorflow/tools:freeze_graph
bazel-bin/tensorflow/tools/freeze_graph \
--input_graph=/tmp/model/my_graph.pb \
--input_checkpoint=/tmp/model/model.ckpt-1000 \
--output_graph=/tmp/frozen_graph.pb \
--output_node_names=output_node \


"""


freeze_graph.freeze_graph(input_graph_path, input_saver_def_path,
                          input_binary, checkpoint_path, output_node_names,
                          restore_op_name, filename_tensor_name,
                          output_frozen_graph_name, clear_devices, "")



# Optimize for inference



"""


If you’ve just been given a frozen GraphDef file, and are not sure about the contents, try using the summarize_graph tool to print out information about the inputs and outputs it finds from the graph structure. Here’s an example with the original Inception v3 file:

bazel run tensorflow/tools/graph_transforms:summarize_graph --
--in_graph=tensorflow_inception_graph.pb

Once you have an idea of what the input and output nodes are, you can feed them into the graph transform tool as the --input_names and --output_names arguments, and call the strip_unused_nodes transform, like this:

bazel run tensorflow/tools/graph_transforms:transform_graph --
--in_graph=tensorflow_inception_graph.pb
--out_graph=optimized_inception_graph.pb --inputs='Mul' --outputs='softmax'
--transforms='
  strip_unused_nodes(type=float, shape="1,299,299,3")
  fold_constants(ignore_errors=true)
  fold_batch_norms
  fold_old_batch_norms'

One thing to look out for here is that you need to specify the size and type that you want your inputs to be. This is because any values that you’re going to be passing in as inputs to inference need to be fed to special Placeholder op nodes, and the transform may need to create them if they don’t already exist. In the case of Inception v3 for example, a Placeholder node replaces the old Mul node that used to output the resized and rescaled image array, since we’re going to be doing that processing ourselves before we call TensorFlow. It keeps the original name though, which is why we always feed in inputs to Mul when we run a session with our modified Inception graph.

After you’ve run this process, you’ll have a graph that only contains the actual nodes you need to run your prediction process. This is the point where it becomes useful to run metrics on the graph, so it’s worth running summarize_graph again to understand what’s in your model.






Optimizing the Model File (this script)

Once we have the frozen graph, we can further optimize the file for inference-only purposes by removing the parts of the graph that are only needed during training. According to the documentation, these include:

    Removing training-only operations like checkpoint saving.
    Stripping out parts of the graph that are never reached.
    Removing debug operations like CheckNumerics.
    Folding batch normalization ops into the pre-calculated weights.
    Fusing common operations into unified versions.

Tensorflow provides optimize_for_inference_lib in tensorflow.python.tools for this purpose:


"""








input_graph_def = tf.GraphDef()
with tf.gfile.Open(output_frozen_graph_name, "r") as f:
    data = f.read()
    input_graph_def.ParseFromString(data)

output_graph_def = optimize_for_inference_lib.optimize_for_inference(
        input_graph_def,
        ["I"], # an array of the input node(s)
        ["O"], # an array of output nodes
        tf.float32.as_datatype_enum)

# Save the optimized graph

f = tf.gfile.FastGFile(output_optimized_graph_name, "w")
f.write(output_graph_def.SerializeToString())

# tf.train.write_graph(output_graph_def, './', output_optimized_graph_name)                    
