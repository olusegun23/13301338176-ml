#!/usr/bin/env python
# -*-coding:utf-8-*-

"""
 #!/usr/bin/env python与#!/usr/bin/python的区别
原创 2012年10月04日 22:46:56

    标签：
    python /
    脚本 /
    语言

脚本语言的第一行，目的就是指出，你想要你的这个文件中的代码用什么可执行程序去运行它，就这么简单

#!/usr/bin/python是告诉操作系统执行这个脚本的时候，调用/usr/bin下的python解释器；
#!/usr/bin/env python这种用法是为了防止操作系统用户没有将python装在默认的/usr/bin路径里。当系统看到这一行的时候，首先会到env设置里查找python的安装路径，再调用对应路径下的解释器程序完成操作。
#!/usr/bin/python相当于写死了python路径;
#!/usr/bin/env python会去环境设置寻找python目录,推荐这种写法

机器学习笔记--python之scipy
原创 2017年09月08日 08:30:21

学习了numpy和matplotlib，基本上线性代数，概率论的很多计算啊之类的都可以很容易的实现了。此外再学习下scipy这个科学函数库吧。
scipy包包含致力于科学计算中常见问题的各个工具箱。它的不同子模块相应于不同的应用。像插值，积分，优化，图像处理，特殊函数等等。
1 模块

scipy 由一些特定功能的子模块组成，它们全依赖numpy,但是每个之间基本独立
模块 	功能
scipy.cluster 	矢量量化 / K-均值
scipy.constants 	物理和数学常数
scipy.fftpack 	傅里叶变换
scipy.integrate 	积分程序
scipy.interpolate 	插值
scipy.io 	数据输入输出
scipy.linalg 	线性代数程序
scipy.ndimage 	n维图像包
scipy.odr 	正交距离回归
scipy.optimize 	优化
scipy.signal 	信号处理
scipy.sparse 	稀疏矩阵
scipy.spatial 	空间数据结构和算法
scipy.special 	任何特殊数学函数
scipy.stats 	统计

之后就学习下部分几个模块吧。


http://blog.csdn.net/eastmoon502136/article/details/77890545
"""


def simpeGen(l):
    for i in l:
        print "====="+str(i)
        yield i

l=[1,2,3,4]
myG=simpeGen(l)
r1=myG.next()
print "r1="+str(r1)
r1=myG.next()
print "r1="+str(r1)

r1=myG.next()
print "r1="+str(r1)

r1=myG.next()
print "r1="+str(r1)


print "--------------------------------------------"


for item in simpeGen(l):
    print item



"""
2 插值 scipy.interpolate

scipy.interpolate对从实验数据拟合函数来求值没有测量值存在的点非常有用
正弦函数的实验


"""



import numpy as np
from scipy.interpolate import interp1d
import pylab as plt

plt.figure(1)



measured_time = np.linspace(0, 1, 10)
noise = (np.random.random(10)*2 - 1) * 1e-1
measures = np.sin(2 * np.pi * measured_time) + noise

#scipy.interpolate.interp1d类会构建线性插值函数：
linear_interp = interp1d(measured_time, measures)
#然后scipy.interpolate.linear_interp实例需要被用来求得感兴趣时间点的值：
computed_time = np.linspace(0, 1, 50)
linear_results = linear_interp(computed_time)
#三次插值也能通过提供可选关键字参数kind来选择
cubic_interp = interp1d(measured_time, measures, kind='cubic')
cubic_results = cubic_interp(computed_time)
#Matplotlib图像中显示如下

plt.subplot(441)
plt.plot(measured_time, measures, 'o', ms=6, label='measures')
plt.plot(computed_time, linear_results, label='linear interp')
plt.plot(computed_time, cubic_results, label='cubic interp')
plt.legend()
plt.show()


"""

3 信号处理 scipy. signal
3.1 移除信号的线性趋势

"""

import numpy as np
import pylab as plt
from scipy import signal

t = np.linspace(0, 3, 200)
x = t + np.random.normal(size=200)

plt.subplot(442)

plt.plot(t, x, linewidth=3)
#移除信号的线性趋势
plt.plot(t, signal.detrend(x), linewidth=3)

plt.show()


"""
3.2FFT重采样

"""

from scipy import signal

t = np.linspace(0, 5, 100)
x = np.sin(t)

plt.subplot(443)
plt.plot(t, x, linewidth=3)
#使用FFT重采样n个点
plt.plot(t[::2], signal.resample(x, 50), 'ko')
plt.show()


"""

4 统计 scipy.stats

scipy.stats包括统计工具和随机过程的概率过程。各个随机过程的随机数生成器可以从numpy.random中找到。
模块 	功能
rvs 	随机变量（就是从这个分布中抽一些样本）
pdf 	概率密度函数
cdf 	累计分布函数
sf 	残存函数（1-CDF）
ppf 	分位点函数（CDF的逆）
isf 	逆残存函数（sf的逆）
stats 	返回均值，方差，（费舍尔）偏态，（费舍尔）峰度
moment 	分布的非中心矩

"""


""".1 二项分布

抛掷10次硬币，假设在该试验中正面朝上的概率为0.3。使用stats.binom.pmf计算每次观测的概率质量函数。

"""

from scipy import stats

plt.subplot(444)
n = 10
p = 0.3
k = np.arange(0, 30)
binomial = stats.binom.pmf(k, n, p)
plt.plot(k, binomial, 'o-')
#使用rvs函数模拟一个二项随机变量，其中参数size指定你要进行模拟的次数，这里为10000次。
plt.subplot(445)
binom_sim = data = stats.binom.rvs(n=10, p=0.3, size=10000)
print "Mean: %g" % np.mean(binom_sim)
print "Sd: %g" % np.std(binom_sim, ddof=1)
plt.hist(binom_sim, bins=10, normed=True)

plt.show()


"""
4.3 正态分布


"""



from scipy import stats


plt.subplot(446)
mu = 0
sigma = 1
x = np.arange(-5, 5, 0.1)
y = stats.norm.pdf(x, 0, 1)
plt.plot(x, y)

plt.subplot(447)

data = stats.norm.rvs(0, 1, size=1000)
plt.hist(data, bins=10, normed=True)

plt.show()


"""

4.4 指数分布

"""

from scipy import stats

plt.subplot(448)
lambd = 0.5
x = np.arange(0, 15, 0.1)
y = lambd*np.exp(-lambd*x)
plt.plot(x, y)

#下模拟1000个随机变量
plt.subplot(449)
data = stats.expon.rvs(scale=2, size=1000)
print "Mean: %g" % np.mean(data)
print "Sd: %g" % np.std(data, ddof=1)
plt.hist(data, bins=20, normed=True)

plt.show()



"""

5 优化和拟合 scipy.optimize

优化是找到最小值或等式的数值解的问题

"""

from scipy import optimize
#定义以下函数x^2+2sin(x)
def f(x):
    return x**2+10*np.sin(x)

x = np.arange(-10, 10, 0.1)

#找到这个函数最小值一般而有效的方法是从初始点使用梯度下降法，这里用拟牛顿法
print optimize.fmin_bfgs(f, 0)

#如果函数有局部最小值，算法会因初始点不同找到这些局部最小而不是全局最小
print optimize.fmin_bfgs(f, 3, disp=0)

#为了找到全局最小点，最简单的算法是蛮力算法
grid = (-10, 10, 0.1)
xmin_global = optimize.brute(f, (grid,))
print xmin_global

#为了找到局部最小，我们把变量限制在(0, 10)之间
xmin_local =optimize.fminbound(f, 0, 10)
print xmin_local

#找到标量函数的根
root1 = optimize.fsolve(f, 1)
print root1

root2 = optimize.fsolve(f, -2)
print root2

#曲线拟合
xdata = np.linspace(-10, 10, num=20)
ydata = f(xdata) + np.random.random(xdata.size)

def f2(x, a, b):
    return a*x**2+b*np.sin(x)

guess = [2, 2]
#通过最小二乘拟合拟合来找到幅度
params, params_convariance = optimize.curve_fit(f2, xdata, ydata, guess)
print params

#画图显示所有的信息

plt.subplot(4,4,10)
plt.plot(x, f(x), c='r', label="f(x)")
plt.plot(xmin_global, f(xmin_global), '^', label='global minima')
plt.plot(xmin_local, f(xmin_local), '^', label='local minima')
plt.plot(root1, f(root1), 'o', label='root1')
plt.plot(root2, f(root2), 'o', label='root2')
plt.plot(x, f2(x, params[0], params[1]), '--', c='b', label="curve fit result")
plt.legend()
plt.show()



"""
http://blog.csdn.net/q583501947/article/details/76735870

Scipy简介

Scipy是一个高级的科学计算库，它和Numpy联系很密切，Scipy一般都是操控Numpy数组来进行科学计算，所以可以说是基于Numpy之上了。Scipy有很多子模块可以应对不同的应用，例如插值运算，优化算法、图像处理、数学统计等。

以下列出Scipy的子模块：
模块名 	功能
scipy.cluster 	向量量化
scipy.constants 	数学常量
scipy.fftpack 	快速傅里叶变换
scipy.integrate 	积分
scipy.interpolate 	插值
scipy.io 	数据输入输出
scipy.linalg 	线性代数
scipy.ndimage 	N维图像
scipy.odr 	正交距离回归
scipy.optimize 	优化算法
scipy.signal 	信号处理
scipy.sparse 	稀疏矩阵
scipy.spatial 	空间数据结构和算法
scipy.special 	特殊数学函数
scipy.stats 	统计函数

"""

"""
文件输入和输出：scipy.io
https://matplotlib.org/users/pyplot_tutorial.html
这个模块可以加载和保存matlab文件：
"""

from scipy import io as spio
a = np.ones((3, 3))
spio.savemat('file.mat', {'a': a}) # 保存字典到file.mat
data = spio.loadmat('file.mat', struct_as_record=True)
print (data['a'])



"""
线性代数操作：scipy.linalg

假如我们要计算一个方阵的行列式，我们需要调用det()函数

"""

from scipy import linalg
arr = np.array([[1, 2],
                [3, 4]])
linalg.det(arr)
arr = np.array([[3, 2],
                 [6, 4]])
print (linalg.det(arr))




"""
快速傅里叶变换：scipy.fftpack

首先我们用numpy初始化正弦信号：

"""



import numpy as np
time_step = 0.02
period = 5.
time_vec = np.arange(0, 20, time_step)
sig = np.sin(2 * np.pi / period * time_vec) + 0.5 * np.random.randn(time_vec.size)



"""
如果我们要计算该信号的采样频率，可以用scipy.fftpack.fftfreq()函数，计算它的快速傅里叶变换使用scipy.fftpack.fft():
"""

from scipy import fftpack
sample_freq = fftpack.fftfreq(sig.size, d=time_step)
sig_fft = fftpack.fft(sig)


"""Numpy中也有用于计算快速傅里叶变换的模块：numpy.fft
但是scipy.fftpack是我们的首选，因为应用了更多底层的工具，工作效率要高一些。关于scipy.fftpack
更多文档。

"""



"""优化器：scipy.optimize

scipy.optimize通常用来最小化一个函数值，我们举个栗子：
构建一个函数并绘制函数图：
"""
def f(x):
    return x**2 + 10*np.sin(x)
x = np.arange(-10, 10, 0.1)

plt.figure(2)
plt.subplot(3,3,1)

plt.plot(x, f(x))
plt.show()



"""一条曲线

如果我们要找出这个函数的最小值，也就是曲线的最低点。就可以用到BFGS优化算法(Broyden–Fletcher–Goldfarb–Shanno algorithm)：
"""

r=optimize.fmin_bfgs(f, 0)


"""可以得到最低点的值为-1.30644003，optimize.fmin_bfgs(f, 0)第二个参数0表示从0点的位置最小化，找到最低点（该点刚好为全局最低点）。假如我从3点的位置开始梯度下降，那么得到的将会是局部最低点 3.83746663：


"""
f=optimize.fmin_bfgs(f, 3, disp=0)


"""假如你无法选出the global minimum的邻近点作为初始点的话可以使用scipy.optimize.basinhopping()
，具体就不展开描述。关于这个模块的其他功能，参考scipy.optimize """


"""

统计工具：scipy.stats

首先我们随机生成1000个服从正态分布的数：

"""

a = np.random.normal(size=1000)
#用stats模块计算该分布的均值和标准差。
loc, std = stats.norm.fit(a)

"""

https://github.com/scipy/scipy/issues/2636

Inconsistency between scipy.stats.mode and scipy.stats.mstats.mode #2636

two mode() functions behave differently when the most frequent value is not unique:

    scipy.stats.mode return the lowest most frequent value
    scipy.stats.mstats.mode return the first most frequent value



"""


import scipy.stats as st
import scipy.stats.mstats as mst

M = np.array([
     [-1,  0,  5],
     [ 7, -1,  3],
     [ 1,  4, -1],
     [ 2,  3,  7],
])

mode_normal = st.mode(M)[0]
mode_masked = np.ma.getdata(mst.mode(M)[0])

#np.testing.assert_array_equal(mode_normal, mode_masked)


"""
https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.mode.html



scipy.stats.mode

scipy.stats.mode(a, axis=0, nan_policy='propagate')[source]

    Return an array of the modal (most common) value in the passed array.

    If there is more than one such value, only the smallest is returned. The bin-count for the modal bins is also returned.
    Parameters:

    a : array_like

        n-dimensional array of which to find mode(s).

    axis : int or None, optional

        Axis along which to operate. Default is 0. If None, compute over the whole array a.

    nan_policy : {‘propagate’, ‘raise’, ‘omit’}, optional

        Defines how to handle when input contains nan. ‘propagate’ returns nan, ‘raise’ throws an error, ‘omit’ performs the calculations ignoring nan values. Default is ‘propagate’.

    Returns:

    mode : ndarray

        Array of modal values.

    count : ndarray

        Array of counts for each mode.

    Examples
    >>>

    >>> a = np.array([[6, 8, 3, 0],
    ...               [3, 2, 1, 7],
    ...               [8, 1, 8, 4],
    ...               [5, 3, 0, 5],
    ...               [4, 7, 5, 9]])
    >>> from scipy import stats
    >>> stats.mode(a)
    (array([[3, 1, 0, 0]]), array([[1, 1, 1, 1]]))

    To get mode of whole array, specify axis=None:
    >>>

    >>> stats.mode(a, axis=None)
    (array([3]), array([3]))




"""


a = np.array([[1, 2, 3, 3],
              [1, 2, 1, 3],
              [3, 3, 1, 3],
             ])

b = np.array([1,4,3,4,4,4,2,4,4])
from scipy import stats

print ("=======================")
print(stats.mode(a))
print(stats.mode(a,axis=0))
print("...[0.0] 1="+str(stats.mode(a,axis=0)[0][0]))

print(stats.mode(a,axis=1))

print ("=======================")

print(stats.mode(a,axis=None))
print("..[0.0]2="+str(stats.mode(a,axis=None)[0][0]))
print(stats.mode(b))
print("...[0.0]3="+str(stats.mode(b)[0][0]))


"""
http://zhenchuan.site/blog/post/admin/from-scipy.stats-import-mode

 from scipy.stats import mode
python 机器学习    2017-05-19    29    0    0

mode: 众数, 即出现次数最多的数.

当axis=None时, 整个array被展开成1维度,就变成了常规可以理解的众数.

当指定axis时, 如axis=0 , 就变成了一个数在列出现的情况统计, 用array[1,columns] 表示. 如果每一列的出现次数大于这一列历史上的最大出现次数,就用当前值(score)替换.

虽然看懂了整个过程,但是依然不明白N维求众数的意义.

        >>> a = np.array([[6, 8, 3, 0],
        ...               [3, 2, 1, 7],
        ...               [8, 1, 8, 4],
        ...               [5, 3, 0, 5],
        ...               [4, 7, 5, 9]])
        >>> from scipy import stats
        >>> stats.mode(a)
        (array([[3, 1, 0, 0]]), array([[1, 1, 1, 1]]))
        >>> stats.mode(a, axis=None)
        (array([3]), array([3]))

    def mode(a, axis=0, nan_policy='propagate'):
        a, axis = _chk_asarray(a, axis)
        ....
        ## 展开数组中的唯一值
        scores = np.unique(np.ravel(a))
        ## [rows,cols]
        testshape = list(a.shape)
        ## row = 1 , [1,cols]
        testshape[axis] = 1
        ## 使用dtype填充testshape为0
        oldmostfreq = np.zeros(testshape, dtype=a.dtype)
        ## 同上,初始count,  Out[55]: array([[0, 0, 0, 0]])
        oldcounts = np.zeros(testshape, dtype=int)
        for score in scores:
            ## 对array中所有的元素应用 a[i]==score, 返回bool类型的array
            template = (a == score)
            ## axis=0表示按列, =1 表示按行, 默认按列
            ## np.sum -> array([1, 1, 1, 0])
            ## np.expand_dims -> array([[1, 1, 1, 0]])
            counts = np.expand_dims(np.sum(template, axis), axis)
            ## counts > oldcounts 返回数组, array([[ True,  True,  True, False]], dtype=bool)
            ## 每个维度出现count最多的把当前数放上去
            mostfrequent = np.where(counts > oldcounts, score, oldmostfreq)
            ## 取counts的最大值
            oldcounts = np.maximum(counts, oldcounts)
            oldmostfreq = mostfrequent
        return ModeResult(mostfrequent, oldcounts)

numpy.where(condition[, x, y])

当conditon的某个位置的为true时，输出x的对应位置的元素，否则选择y对应位置的元素；
numpy.expand_dims(a, axis)

    x = np.array([1,1,1,0])
    x.shape
    Out[73]: (4,)
    ## 一行4[列], 0 表示列
    np.expand_dims(x,0)
    Out[57]: array([[1, 1, 1, 0]])
    ## 4行一列
    np.expand_dims(x,1)
    Out[59]:
    array([[1],
           [1],
           [1],
           [0]])

    def _chk_asarray(a, axis):
        if axis is None:
            ## 展开,变1维
            a = np.ravel(a)
            outaxis = 0
        else:
            ## 保持不变
            a = np.asarray(a)
            outaxis = axis
        if a.ndim == 0:  ## 0 维
            a = np.atleast_1d(a)
        return a, outaxis

0维情况如下:

    b = np.array(None)
    np.atleast_1d(b)
    Out[37]: array([None], dtype=object)

Pre: Ambari heartbeat lost

Next: FMM源码阅读


"""


