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

import numpy as np;


# coding=utf-8
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import cm
from matplotlib import axes
from matplotlib import mpl
import csv

"""

def draw_heatmap(data, xlabels, ylabels):
    # cmap=cm.Blues
    cmap = cm.get_cmap('rainbow', 1000)
    figure = plt.figure(facecolor='w')
    ax = figure.add_subplot(1, 1, 1, position=[0.1, 0.15, 0.8, 0.8])
    ax.set_yticks(range(len(ylabels)))
    ax.set_yticklabels(ylabels)
    ax.set_xticks(range(len(xlabels)))
    ax.set_xticklabels(xlabels)
    vmax = data[0][0]
    vmin = data[0][0]
    for i in data:
        for j in i:
            if j > vmax:
                vmax = j
            if j < vmin:
                vmin = j
    map = ax.imshow(data, interpolation='nearest', cmap=cmap, aspect='auto', vmin=vmin, vmax=vmax)
    cb = plt.colorbar(mappable=map, cax=None, ax=None, shrink=0.5)
    plt.show()




a = np.random.rand(3, 3)
print a
xlabels = ['A', 'B', 'C']
ylabels = ['a', 'b', 'c']
draw_heatmap(a, xlabels, ylabels)

b=[[1,2,3,4],[5,6,7,8],[9,10,11,12]]


xlabels = ['COL1', 'COL2', 'COL3','COL4']
ylabels = ['row1', 'row2', 'row2']
draw_heatmap(b, xlabels, ylabels)


"""


plt.figure(1)

url = "/home/haijunz/1/mbean/my/still/L_-10_2__d7c5ebd8a4756d6f_ACC_2018-01-09-21-25-40:666000000.csv"

names = ['NA', 'time','time1','time2', 'acc_x', 'acc_y', 'acc_z']
names1 = ['acc_x', 'acc_y', 'acc_z']
#Signature:main:(id)2:(priority)5:(group)main||my thread_id2:-10_d7c5ebd8a4756d6f_Acc,23028731613574,2018-01-07-23-05-35:908000000,2018-01-07-23-05-35:919000000,-1.3196558,6.006709,6.821016
dataset = pandas.read_csv(url, names=names)
data=dataset[names1]
# shape
print(dataset.shape)
# head
print(dataset.head(20))
# descriptions
#desc=data.describe()
#plt.plot(desc)
#plt.show()
#print (desc.dtypes)
print(dataset.describe())

#y = pandas.to_datetime(dataset["time1"], format="%Y-%m-%d-%H-%M-%S:%f")
y = dataset["time"]
#d =y-y.shift()
c=np.diff(np.array(y))
a1 = plt.subplot(2, 2, 1)
a1.plot(c/1000000)
plt.xlabel("samples")# plots an axis lable
plt.ylabel("interval/ms")
plt.title("Interval")


x = range(len(dataset["time"].values))
y = len(dataset["time"].values)
#pd.to_datetime('2018-01-07-23-05-35:908000000', format="%Y-%m-%d-%H-%M-%S:%f")
assert (len(x) == len(dataset["acc_x"].values))

a1 = plt.subplot(2, 2, 2)
a1.plot(x, dataset["acc_x"], label="acc_x")
a1.plot(x, dataset["acc_y"], label="acc_y")
a1.plot(x, dataset["acc_z"], label="acc_z")
plt.legend(loc='best')

plt.xlabel("samples")# plots an axis lable
plt.ylabel("values")
plt.title("Still on desktop Acc")
plt.show()




plt.figure(2)


url = "/home/haijunz/1/mbean/my/still/L_-10_2__d7c5ebd8a4756d6f_Gyr_2018-01-09-21-25-40:666000000.csv"

names = ['NA', 'time','time1','time2', 'Gyr_x', 'Gyr_y', 'Gyr_z']

dataset = pandas.read_csv(url, names=names)

# shape
print(dataset.shape)
print(dataset.head(20))
print(dataset.describe())
y = dataset["time"]
c=np.diff(np.array(y))
a1 = plt.subplot(2, 2, 1)
a1.plot(c/1000000)
plt.xlabel("samples")# plots an axis lable
plt.ylabel("interval/ms")
plt.title("Interval")


x = range(len(dataset["time"].values))
y = len(dataset["time"].values)
assert (len(x) == len(dataset["Gyr_x"].values))
a1 = plt.subplot(2, 2, 2)
a1.plot(x, dataset["Gyr_x"], label="Gyr_x")
a1.plot(x, dataset["Gyr_y"], label="Gyr_y")
a1.plot(x, dataset["Gyr_z"], label="Gyr_z")
plt.legend(loc='best')

plt.xlabel("samples")# plots an axis lable
plt.ylabel("values")
plt.title("Still on desktop Gyr")
plt.show()



plt.figure(3)


url = "/home/haijunz/1/mbean/my/still/L_-10_2__d7c5ebd8a4756d6f_Mag_2018-01-09-21-25-40:666000000.csv"

names = ['NA', 'time','time1','time2', 'Mag_x', 'Mag_y', 'Mag_z']

dataset = pandas.read_csv(url, names=names)

# shape
print(dataset.shape)
print(dataset.head(20))
print(dataset.describe())
y = dataset["time"]
c=np.diff(np.array(y))
a1 = plt.subplot(2, 2, 1)
a1.plot(c/1000000)
plt.xlabel("samples")# plots an axis lable
plt.ylabel("interval/ms")
plt.title("Interval")


x = range(len(dataset["time"].values))
y = len(dataset["time"].values)
assert (len(x) == len(dataset["Mag_x"].values))
a1 = plt.subplot(2, 2, 2)
a1.plot(x, dataset["Mag_x"], label="Mag_x")
a1.plot(x, dataset["Mag_y"], label="Mag_y")
a1.plot(x, dataset["Mag_z"], label="Mag_z")
plt.legend(loc='best')

plt.xlabel("samples")# plots an axis lable
plt.ylabel("values")
plt.title("Still on desktop Mag")
plt.show()





















# class distribution
#print(dataset.groupby('class').size())

#types = dataset.dtypes
#print(types)

#correlations = dataset.corr()
#print(correlations)

#skew = dataset.skew()
#print(skew)












"""
fig = plt.figure()
ax = fig.add_subplot(223)
cmap=mpl.cm.hot
im=ax.imshow(data,cmap=cmap)
plt.colorbar(im)
"""








"""



fig = plt.figure(3)
a = np.array([0.313660827978, 0.365348418405, 0.423733120134,
              0.365348418405, 0.439599930621, 0.525083754405,
              0.423733120134, 0.525083754405, 0.651536351379]).reshape(3,3)

plt.imshow(a, cmap='hot')

"""




"""


fig = plt.figure(4)




plt.hist2d(dataset["acc_y"],dataset["acc_x"], bins=2, cmap='hot')

plt.colorbar()





y = pandas.to_datetime(dataset["time1"], format="%Y-%m-%d-%H-%M-%S:%f")

d =y-y.shift()


b = np.array(y)
c=np.diff(b)
plt.plot(c/1000000)
plt.show()

x = range(len(dataset["time"].values))
y = len(dataset["time"].values)
#pd.to_datetime('2018-01-07-23-05-35:908000000', format="%Y-%m-%d-%H-%M-%S:%f")
assert (len(x) == len(dataset["acc_x"].values))

a1 = plt.subplot(2, 2, 1)
a1.plot(x, dataset["acc_x"], label="acc_x")
a1.plot(x, dataset["acc_y"], label="acc_y")
a1.plot(x, dataset["acc_z"], label="acc_z")
plt.legend(loc='best')


plt.show()




"""


"""


https://seaborn.pydata.org/tutorial/categorical.html




import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
import seaborn as sns
import numpy as np













# too big slow  ====good
# sns.pairplot(dataset, hue='class');




ax = sns.barplot(x="timestamp", y="acc_x", hue="class", data=dataset[1:10])

# sns.countplot(y='acc_x', data=dataset[1:10], hue='class')










# ax = dataset.plot(kind='bar', rot=0)
# for label in ax.get_xticklabels():



# dataset.hist(by='class')

# dataset.groupby('class').hist(subplots=True)

# dataset.plot(kind='hist', subplots=True, sharex=True, sharey=True, title='My title')



# plt.show()



# plt.figure()
# sns.stripplot( x="class",y="acc_x", data=dataset);
# plt.figure()
# dataset['class'].value_counts().plot(kind='bar')
# plt.show()






# box and whisker plots
# dataset.plot(kind='box', subplots=True, layout=(4, 4), sharex=False, sharey=False)
# plt.show()


# histograms
# dataset.hist()
# plt.show()

# scatter plot matrix
# scatter_matrix(dataset)
# plt.show()












https://www.swharden.com/wp/2009-06-19-reading-pcm-audio-with-python/




"""












"""


import numpy, pylab

data = numpy.memmap("2018-01-07-23-21-47:687000000_16000_1.pcm", dtype='h', mode='r')

print (data)

pylab.plot(data)
pylab.show()



https://www.swharden.com/wp/2009-01-21-signal-filtering-with-python/

"""
