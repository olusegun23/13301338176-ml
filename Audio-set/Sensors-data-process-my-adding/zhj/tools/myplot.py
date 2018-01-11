#!/usr/bin/env python
# -*-coding:utf-8-*-

"""
然后需要调用函数绘图，下面提几个重点知识：
    1.plot(x, y, marker='D')表示绘制折线图，marker设置样式菱形。
    2.scatter(x, y, marker='s', color='r')绘制散点图，红色正方形。
    3.bar(x, y, 0.5, color='c')绘制柱状图，间距为0.5，原色。
    4.hist(data,40,normed=1,histtype='bar',
                   facecolor='yellowgreen',alpha=0.75)直方图。
    5.设置x轴和y轴的坐标值：
      xlim(-2.5, 2.5) #设置x轴范围 ylim(-1, 1) #设置y轴范围
    6.显示中文和负号代码如下：
      plt.rcParams['font.sas-serig']=['SimHei'] #用来正常显示中文标签
      plt.rcParams['axes.unicode_minus']=False #用来正常显示负号
"""

from pylab import *



import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib

# coding=utf-8
import numpy as np
from pylab import *
from matplotlib import pyplot as plt



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




x = [1, 2, 3, 4]
y = [3, 5, 10, 25]

# 创建Figure
fig = plt.figure()

# 创建一个或多个子图(subplot绘图区才能绘图)
ax1 = fig.add_subplot(231)
plt.plot(x, y, marker='D')  # 绘图及选择子图
plt.sca(ax1)

ax2 = fig.add_subplot(232)
plt.scatter(x, y, marker='s', color='r')
plt.sca(ax2)
plt.grid(True)

ax3 = fig.add_subplot(233)
plt.bar(x, y, 0.5, color='c')  # 柱状图 width=0.5间距
plt.sca(ax3)

ax4 = fig.add_subplot(234)
# 高斯分布
mean = 0  # 均值为0
sigma = 1  # 标准差为1 (反应数据集中还是分散的值)
data = mean + sigma * np.random.randn(10000)
plt.hist(data, 40, normed=1, histtype='bar', facecolor='yellowgreen', alpha=0.75)
plt.sca(ax4)

m = np.arange(-5.0, 5.0, 0.02)
n = np.sin(m)
ax5 = fig.add_subplot(235)
plt.plot(m, n)
plt.sca(ax5)

ax6 = fig.add_subplot(236)
xlim(-2.5, 2.5)  # 设置x轴范围
ylim(-1, 1)  # 设置y轴范围
plt.plot(m, n)
plt.sca(ax6)
plt.grid(True)

plt.show()




















data_train=pd.read_csv('titanic_train.csv')

print data_train.head(10)

plt.plot(data_train.Age)

##现在我们想看看乘客年龄分布，kde就是密度分布，类似于直方图，数据落在在每个bin内的频率大小或者是密度大小
data_train.Age.plot(kind='kde')
plt.xlabel(u"年龄")# plots an axis lable
plt.ylabel(u"密度")
plt.title(u"乘客年龄分布")

##现在看看获救人数和未获救人数对比
#plt.subplot2grid((2,3),(0,0))
data_train.Survived.value_counts().plot(kind='bar')# plots a bar graph of those who surived vs those who did not.
plt.title(u"获救情况 (1为获救)") # puts a title on our graph
plt.ylabel(u"人数")


## 常看各乘客等级的获救情况
Survived_0 = data_train.Pclass[data_train.Survived == 0].value_counts()
Survived_1 = data_train.Pclass[data_train.Survived == 1].value_counts()
df=pd.DataFrame({u'获救':Survived_1, u'未获救':Survived_0})
df.plot(kind='bar', stacked=True)
plt.title(u"各乘客等级的获救情况")
plt.xlabel(u"乘客等级")
plt.ylabel(u"人数")

##再来看看各种级别舱情况下性别的获救情况
fig=plt.figure()
fig.set(alpha=0.65) # 设置图像透明度，无所谓
plt.title(u"根据舱等级和性别的获救情况")

ax1=fig.add_subplot(141)
data_train.Survived[data_train.Sex == 'female'][data_train.Pclass != 3].value_counts().plot(kind='bar', label="female highclass", color='#FA2479')
ax1.set_xticklabels([u"获救", u"未获救"], rotation=0)
ax1.legend([u"女性/高级舱"], loc='best')

ax2=fig.add_subplot(142, sharey=ax1)
data_train.Survived[data_train.Sex == 'female'][data_train.Pclass == 3].value_counts().plot(kind='bar', label='female, low class', color='pink')
ax2.set_xticklabels([u"未获救", u"获救"], rotation=0)
plt.legend([u"女性/低级舱"], loc='best')

ax3=fig.add_subplot(143, sharey=ax1)
data_train.Survived[data_train.Sex == 'male'][data_train.Pclass != 3].value_counts().plot(kind='bar', label='male, high class',color='lightblue')
ax3.set_xticklabels([u"未获救", u"获救"], rotation=0)
plt.legend([u"男性/高级舱"], loc='best')

ax4=fig.add_subplot(144, sharey=ax1)
data_train.Survived[data_train.Sex == 'male'][data_train.Pclass == 3].value_counts().plot(kind='bar', label='male low class', color='steelblue')
ax4.set_xticklabels([u"未获救", u"获救"], rotation=0)
plt.legend([u"男性/低级舱"], loc='best')



import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

fig=plt.figure()
ax=fig.add_subplot(1,1,1)
ax.plot(np.arange(0,100,10),np.random.randn(10).cumsum(),marker='o')
ax.set_xticks([0,10,20,30,40,50,60,70,80,90]) ##设置x轴上显示的刻度
ax.grid()##　显示方格
plt.show()



##现在我们想看看每个等级的舱的乘客的平均年龄
data_train.groupby('Pclass').mean().plot(y='Age',marker='o')
##注意参数marker='o'强调实际的数据点，会在实际的数据点上加一个实心点。如果要显示方格可在plot里面设置参数grid=True
plt.xlabel(u"舱级别")
plt.ylabel(u"平均年龄")



##也可以这样看看年龄和所在舱级别的关系
data_train.plot(x='Pclass',y='Age',kind='scatter')
plt.xlabel(u"舱级别")
plt.ylabel(u"年龄")
plt.show()




"""




"""


