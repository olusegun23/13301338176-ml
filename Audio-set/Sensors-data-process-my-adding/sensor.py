#!/usr/bin/env python
# -*-coding:utf-8-*-

"""

Implements
"""

# Python version
import sys
print('Python: {}'.format(sys.version))
# scipy
import scipy
print('scipy: {}'.format(scipy.__version__))
# numpy
import numpy as np
print('numpy: {}'.format(np.__version__))
# matplotlib
import matplotlib
print('matplotlib: {}'.format(matplotlib.__version__))
# pandas

import pandas
print('pandas: {}'.format(pandas.__version__))
# scikit-learn
import sklearn
print('sklearn: {}'.format(sklearn.__version__))



# Load libraries
import pandas
from pandas.tools.plotting import scatter_matrix
import matplotlib.pyplot as plt
from sklearn import model_selection
from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix
from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC



from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import minmax_scale
from sklearn.preprocessing import MaxAbsScaler
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import RobustScaler
from sklearn.preprocessing import Normalizer

import matplotlib as mpl;
import matplotlib.pyplot as plt;



class Sensor(object):
    def load_python_csv(self):
        import csv
        a = []
        pattern = {'huangy-incar01': 1001, 'huangy-pantpocket': 1002, 'UNKNOW': 1003, 'zhj_incar_packet': 1004,
                   'zhj_incar_hands': 1005, 'Stillontable': 1006}
        with open('20171226.csv', 'rb') as f:  # 采用b的方式处理可以省去很多问题
            reader = csv.reader(f)
            for row in reader:
                if (len(row) > 10):
                    rep = [float(pattern[x]) if x in pattern else float(x) for x in row]
                    a.append(rep)



        with open('20171226-clean.csv', 'wb') as csvfile:
            writer = csv.writer(csvfile)
            for row in a:
                writer.writerow(row)





        data2 = [[1, 2, 3, 4], [5, 6, 7, 8]]
        arr2 = np.array(data2)

        n=np.array(a)

        names = ['timestamp', 't','x', 'y', 'z','t1','x1', 'y1', 'z1','t2','x2', 'y2', 'z2', 'class']
        dataset = pandas.DataFrame(n, columns=names)

        print dataset.describe()



        print 'huangy-incar01'

    def load_panda_csv(self):
        #url = "https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"
        url='20171226-clean.csv'
        names = ['timestamp', 't', 'x', 'y', 'z', 't1', 'x1', 'y1', 'z1', 't2', 'x2', 'y2', 'z2', 'class']
        dataset = pandas.read_csv(url, names=names,dtype=np.float32,verbose=True)

        tensor=dataset.as_matrix()

        print dataset.describe()

    def example_panda_csv(self):
        import pandas as pd
        import numpy as np

        raw_data = {'first_name': ['Jason', 'Molly', 'Tina', 'Jake', 'Amy'],
                    'last_name': ['Miller', 'Jacobson', ".", 'Milner', 'Cooze'],
                    'age': [42, 52, 36, 24, 73],
                    'preTestScore': [4, 24, 31, ".", "."],
                    'postTestScore': ["25,000", "94,000", 57, 62, 70]}
        df = pd.DataFrame(raw_data, columns=['first_name', 'last_name', 'age', 'preTestScore', 'postTestScore'])
        df
        df.to_csv('example.csv')
        df = pd.read_csv('example.csv')
        df

        df = pd.read_csv('example.csv', header=None)
        df

        df = pd.read_csv('example.csv',
                         names=['UID', 'First Name', 'Last Name', 'Age', 'Pre-Test Score', 'Post-Test Score'])
        df

        df = pd.read_csv('example.csv', index_col='UID',
                         names=['UID', 'First Name', 'Last Name', 'Age', 'Pre-Test Score', 'Post-Test Score'])
        df

        df = pd.read_csv('example.csv', index_col=['First Name', 'Last Name'],
                         names=['UID', 'First Name', 'Last Name', 'Age', 'Pre-Test Score', 'Post-Test Score'])
        df

        df = pd.read_csv('example.csv', na_values=['.'])
        pd.isnull(df)

        sentinels = {'Last Name': ['.', 'NA'], 'Pre-Test Score': ['.']}

        df = pd.read_csv('example.csv', na_values=sentinels)
        df

        df = pd.read_csv('example.csv', na_values=sentinels, skiprows=3)
        df

        df = pd.read_csv('example.csv', thousands=',')
        df


    """
    def load_tf_csv(self):
        import tensorflow as tf
        filename_queue = tf.train.string_input_producer(["20171226.csv"])

        reader = tf.TextLineReader()
        key, value = reader.read(filename_queue)

        # Default values, in case of empty columns. Also specifies the type of the
        # decoded result.
        record_defaults = [[1923], [3], [0.0], [0.0], 0.0],[4], [0], [0], [0], [5],[0], [0], [0], ["UNKNOW"]]
        col1, col2, col3, col4, col5, col6, col7, col8, col9, col10,  col11, col12, col13, col4= tf.decode_csv(value, record_defaults=record_defaults)
        features = tf.concat(0, [col1,col2, col3, col4, col5, col6, col7, col8, col9,  col10, col11, col2, col3, col4])

        with tf.Session() as sess:
            # Start populating the filename queue.
            coord = tf.train.Coordinator()
            threads = tf.train.start_queue_runners(coord=coord)

            for i in range(1200):
                # Retrieve a single instance:
                example, label = sess.run([features, col5])

            coord.request_stop()
            coord.join(threads)

    """

    def csv_panda_tf(self):
        import tensorflow as tf
        import numpy
        import pandas as pd
        df = pd.read_csv('iris.csv', usecols=[0, 1, 2, 3, 4], skiprows=[0], header=None)
        d = df.values

        lables = d[:,-1]
        d1= d[:,1:-1]

        data = numpy.float32(d1)
        labels = numpy.array(lables, 'str')
        # print data, labels

        # tensorflow
        x = tf.placeholder(tf.float32, shape=(150, 5))
        x = data
        w = tf.random_normal([100, 150], mean=0.0, stddev=1.0, dtype=tf.float32)
        y = tf.nn.softmax(tf.matmul(w, x))

        with tf.Session() as sess:
            print sess.run(y)





def run():

    sensor= Sensor()
    #sensor.load_python_csv()
    #sensor.load_panda_csv()
    #sensor.example_panda_csv()
    sensor.csv_panda_tf()




if __name__ == "__main__":
    run()










