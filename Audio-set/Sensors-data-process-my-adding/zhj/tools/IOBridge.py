#!/usr/bin/env python
# -*-coding:utf-8-*-

"""

Implements
"""

from __future__ import print_function

import math

from pandas.plotting import radviz


class IOBridge(object):

    def __init__(self):
        self.x=100


    def show(self):
        print("===========")

    def demo(self, a, b, c):

        d = b ** 2 - 4 * a * c

        if d >= 0:

            disc = math.sqrt(d)

            root1 = (-b + disc) / (2 * a)

            root2 = (-b - disc) / (2 * a)

            self.show()
            print(root1, root2)

        else:

            raise Exception

    def ScikitData(self):

        """
         http://scikit-learn.org/stable/auto_examples/plot_feature_stacker.html#sphx-glr-auto-examples-plot-feature-stacker-py

        """

        from sklearn.pipeline import Pipeline, FeatureUnion
        from sklearn.model_selection import GridSearchCV
        from sklearn.svm import SVC
        from sklearn.datasets import load_iris
        from sklearn.decomposition import PCA
        from sklearn.feature_selection import SelectKBest

        iris = load_iris()

        X, y = iris.data, iris.target

        # This dataset is way too high-dimensional. Better do PCA:
        pca = PCA(n_components=2)

        # Maybe some original features where good, too?
        selection = SelectKBest(k=1)

        # Build estimator from PCA and Univariate selection:

        combined_features = FeatureUnion([("pca", pca), ("univ_select", selection)])

        # Use combined features to transform dataset:
        X_features = combined_features.fit(X, y).transform(X)

        svm = SVC(kernel="linear")

        # Do grid search over k, n_components and C:

        pipeline = Pipeline([("features", combined_features), ("svm", svm)])

        param_grid = dict(features__pca__n_components=[1, 2, 3],
                          features__univ_select__k=[1, 2],
                          svm__C=[0.1, 1, 10])

        grid_search = GridSearchCV(pipeline, param_grid=param_grid, verbose=10)
        grid_search.fit(X, y)
        print(grid_search.best_estimator_)










    def FirstMLStepByStep(self):
        """
         https://machinelearningmastery.com/machine-learning-in-python-step-by-step/

         """
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

        #url = "https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"
        url = "iris.data"
        names = ['sepal-length', 'sepal-width', 'petal-length', 'petal-width', 'class']
        dataset = pandas.read_csv(url, names=names)
        # shape
        print(dataset.shape)

        # head
        print(dataset.head(20))
        # descriptions
        print(dataset.describe())
        # class distribution
        print(dataset.groupby('class').size())


        # box and whisker plots
        dataset.plot(kind='box', subplots=True, layout=(2, 2), sharex=False, sharey=False)
        plt.show()


        # histograms
        dataset.hist()
        plt.show()

        # scatter plot matrix
        scatter_matrix(dataset)
        plt.show()



        # Split-out validation dataset
        array = dataset.values

        X = array[:, 0:4]
        Y = array[:, 4]
        validation_size = 0.20
        seed = 7
        X_train, X_validation, Y_train, Y_validation = model_selection.train_test_split(X, Y, test_size=validation_size,
                                                                                        random_state=seed)


        # Test options and evaluation metric
        seed = 7
        scoring = 'accuracy'

        # Spot Check Algorithms
        models = []
        models.append(('LR', LogisticRegression()))
        models.append(('LDA', LinearDiscriminantAnalysis()))
        models.append(('KNN', KNeighborsClassifier()))
        models.append(('CART', DecisionTreeClassifier()))
        models.append(('NB', GaussianNB()))
        models.append(('SVM', SVC()))
        # evaluate each model in turn
        results = []
        names = []
        for name, model in models:
            kfold = model_selection.KFold(n_splits=10, random_state=seed)
            cv_results = model_selection.cross_val_score(model, X_train, Y_train, cv=kfold, scoring=scoring)
            results.append(cv_results)
            names.append(name)
            msg = "%s: %f (%f)" % (name, cv_results.mean(), cv_results.std())
            print(msg)

        # Compare Algorithms
        fig = plt.figure()
        fig.suptitle('Algorithm Comparison')
        ax = fig.add_subplot(111)
        plt.boxplot(results)
        ax.set_xticklabels(names)
        plt.show()

        # Make predictions on validation dataset
        knn = KNeighborsClassifier()
        knn.fit(X_train, Y_train)
        predictions = knn.predict(X_validation)
        print(accuracy_score(Y_validation, predictions))
        print(confusion_matrix(Y_validation, predictions))
        print(classification_report(Y_validation, predictions))



    def SensorMLStepByStep(self):
        """
         https://machinelearningmastery.com/machine-learning-in-python-step-by-step/

         """
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

        #url = "https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"

        import csv

        """


        a = []
        pattern = {'huangy-incar01': 1001, 'huangy-pantpocket': 1002, 'UNKNOW': 1003, 'zhj_incar_packet': 1004,
                   'zhj_incar_hands': 1005, 'Stillontable': 1006}
        with open('20171226.csv', 'rb') as f:  # 采用b的方式处理可以省去很多问题
            reader = csv.reader(f)
            for row in reader:
                if (len(row) > 10):
                    a.append(row)



        with open('20171226-clean.csv', 'wb') as csvfile:
            writer = csv.writer(csvfile)
            for row in a:
                writer.writerow(row)

        """

        url = "20171226-clean.csv"

        names = ['timestamp', 'acc', 'acc_x', 'acc_y', 'acc_z', 'Gyr', 'Gyr_x', 'Gyr_y', 'Gyr_z', 'mag', 'mag_x', 'mag_y', 'mag_z', 'class']

        dataset = pandas.read_csv(url, names=names)

        x=range(len(dataset["timestamp"].values))
        y=len(dataset["timestamp"].values)

        assert(len(x)==len(dataset["acc_x"].values))

        a1 = plt.subplot(2, 2, 1)
        a1.plot(x,dataset["acc_x"],label="acc_x")
        a1.plot(x, dataset["acc_y"],label="acc_y")
        a1.plot(x, dataset["acc_z"],label="acc_z")
        plt.legend(loc='best')
        a2 = plt.subplot(2, 2, 2)
        a2.plot(x, dataset["Gyr_x"], label="Gyr_x")
        a2.plot(x, dataset["Gyr_y"], label="Gyr_y")
        a2.plot(x, dataset["Gyr_z"], label="Gyr_z")
        plt.legend(loc='best')
        a3 = plt.subplot(2, 2, 3)
        a3.plot(x, dataset["mag_x"], label="mag_x")
        a3.plot(x, dataset["mag_y"], label="mag_y")
        a3.plot(x, dataset["mag_z"], label="mag_z")
        plt.legend(loc='best')

        plt.show()

        names1 = ['acc', 'acc_x', 'acc_y', 'acc_z', 'Gyr', 'Gyr_x', 'Gyr_y', 'Gyr_z', 'mag', 'mag_x','mag_y', 'mag_z', 'class']
        dataset1=dataset[names1]




        # shape
        print(dataset.shape)

        # head
        print(dataset.head(20))

        print(dataset1.head(20))

        # descriptions
        print(dataset.describe())
        # class distribution
        print(dataset.groupby('class').size())

        types = dataset.dtypes
        print(types)

        correlations = dataset.corr()
        print(correlations)

        skew = dataset.skew()
        print(skew)



        """
        https://seaborn.pydata.org/tutorial/categorical.html



        """
        import numpy as np
        import pandas as pd
        import matplotlib as mpl
        import matplotlib.pyplot as plt
        import seaborn as sns
        import seaborn as sns
        import numpy as np



        #too big slow  ====good
        # sns.pairplot(dataset, hue='class');



        #sns.set_style("whitegrid")
        #tips = sns.load_dataset("tips")  # 载入自带数据集
        # x轴为分类变量day,y轴为数值变量total_bill，利用颜色再对sex分类
        ax = sns.barplot(x="timestamp", y="acc_x", hue="class", data=dataset[1:10])

        #sns.countplot(y='acc_x', data=dataset[1:10], hue='class')









        # 如下图2(DF)
        #ax = dataset.plot(kind='bar', rot=0)
        #for label in ax.get_xticklabels():



        #dataset.hist(by='class')

        #dataset.groupby('class').hist(subplots=True)

        #dataset.plot(kind='hist', subplots=True, sharex=True, sharey=True, title='My title')



        #plt.show()



        #plt.figure()
        #sns.stripplot( x="class",y="acc_x", data=dataset);
        #plt.figure()
        #dataset['class'].value_counts().plot(kind='bar')
        #plt.show()






        # box and whisker plots
        #dataset.plot(kind='box', subplots=True, layout=(4, 4), sharex=False, sharey=False)
        #plt.show()


        # histograms
        #dataset.hist()
        #plt.show()

        # scatter plot matrix
        #scatter_matrix(dataset)
        #plt.show()



        """borken Density   Plots"""

        #dataset.plot(kind='density', subplots=True, layout=(4, 4), sharex=False, sharey=False)
        #plt.show()






        # plot correlation matrix

        correlations = dataset.corr()
        # plot correlation matrix
        fig = plt.figure()
        ax = fig.add_subplot(111)
        cax = ax.matshow(correlations, vmin=-1, vmax=1)
        fig.colorbar(cax)
        ticks = np.arange(0, 14, 1)
        ax.set_xticks(ticks)
        ax.set_yticks(ticks)
        ax.set_xticklabels(names)
        ax.set_yticklabels(names)
        #plt.show()


        # Split-out validation dataset
        array = dataset.values

        array_float = []
        for row in array:
            row1=row[:-1]
            rep = [float(x) for x in row1]
            array_float.append(rep)

        X = array[:, 1:13]
        Y =array[:, 13]
        validation_size = 0.20








        seed = 7
        X_train, X_validation, Y_train, Y_validation = model_selection.train_test_split(X, Y, test_size=validation_size,
                                                                                        random_state=seed)




        from sklearn.model_selection import train_test_split
        from sklearn.linear_model import LogisticRegressionCV

        from keras.models import Sequential
        from keras.layers.core import Dense, Activation
        from keras.utils import np_utils

        def one_hot_encode_object_array(arr):
            '''One hot encode a numpy array of objects (e.g. strings)'''
            uniques, ids = np.unique(arr, return_inverse=True)
            return np_utils.to_categorical(ids, len(uniques))

        train_y_ohe = one_hot_encode_object_array(Y_train)
        test_y_ohe = one_hot_encode_object_array(Y_validation)

        model = Sequential()
        model.add(Dense(16, input_shape=(12,)))
        model.add(Activation('sigmoid'))
        model.add(Dense(6))
        model.add(Activation('softmax'))
        model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=["accuracy"])
        print ("===epoch=10 neural network Accuracy = 0.95=====\n")
        print("Keras====neural network before========\n")
        model.fit( X_train, train_y_ohe, epochs=100, batch_size=1, verbose=1);

        loss, accuracy = model.evaluate(X_validation, test_y_ohe, verbose=1)
        print("Keras====neural network Accuracy = {:.2f} \n".format(accuracy))








        # Test options and evaluation metric
        seed = 7
        scoring = 'accuracy'

        # Spot Check Algorithms
        models = []
        models.append(('LR', LogisticRegression()))
        models.append(('LDA', LinearDiscriminantAnalysis()))
        models.append(('KNN', KNeighborsClassifier()))
        models.append(('CART', DecisionTreeClassifier()))
        models.append(('NB', GaussianNB()))
        models.append(('SVM', SVC()))
        # evaluate each model in turn
        results = []
        names = []
        for name, model in models:
            kfold = model_selection.KFold(n_splits=10, random_state=seed)
            cv_results = model_selection.cross_val_score(model, X_train, Y_train, cv=kfold, scoring=scoring)
            results.append(cv_results)
            names.append(name)
            msg = "%s: %f (%f)" % (name, cv_results.mean(), cv_results.std())
            print(msg)

        # Compare Algorithms
        fig = plt.figure()
        fig.suptitle('Algorithm Comparison')
        ax = fig.add_subplot(111)
        plt.boxplot(results)
        ax.set_xticklabels(names)
        plt.show()



        # Make predictions on validation dataset
        knn = KNeighborsClassifier()
        knn.fit(X_train, Y_train)
        predictions = knn.predict(X_validation)
        print(accuracy_score(Y_validation, predictions))
        print(confusion_matrix(Y_validation, predictions))
        print(classification_report(Y_validation, predictions))




        from sklearn.preprocessing import MinMaxScaler
        scaler = MinMaxScaler(feature_range=(0, 1))
        rescaledX = scaler.fit_transform(X)

        from sklearn.preprocessing import StandardScaler
        from numpy import set_printoptions
        scaler = StandardScaler().fit(X)
        rescaledX = scaler.transform(X)

        # Normalize data (length of 1)
        from sklearn.preprocessing import Normalizer
        scaler = Normalizer().fit(X)
        normalizedX = scaler.transform(X)
















    def PdCategoricalData(self):
        """
           https://chrisalbon.com/machine-learning/convert_pandas_categorical_column_into_integers_for_scikit-learn.html
        """
        from sklearn import preprocessing
        import pandas as pd
        raw_data = {'patient': [1, 1, 1, 2, 2],
                   'obs': [1, 2, 3, 1, 2],
                   'treatment': [0, 1, 0, 1, 0],
                   'score': ['strong', 'weak', 'normal', 'weak', 'strong']}
        df = pd.DataFrame(raw_data, columns=['patient', 'obs', 'treatment', 'score'])
        # Create a label (category) encoder object
        le = preprocessing.LabelEncoder()
        # Fit the encoder to the pandas column
        le.fit(df['score'])
        # View the labels (if you want)
        list(le.classes_)
        # Apply the fitted encoder to the pandas column
        le.transform(df['score'])
        # Convert some integers into their category names
        list(le.inverse_transform([2, 2, 1]))

    def npVSpdVSTorch(self):
        """
        https://morvanzhou.github.io/tutorials/data-manipulation/np-pd/2-6-np-concat/
        https://morvanzhou.github.io/tutorials/machine-learning/sklearn/3-1-normalization/
         莫烦Python 并看到更好的教学内


        """
        import numpy as np


        A = np.array([1, 1, 1])
        B = np.array([2, 2, 2])

        C = np.vstack((A, B))
        print(A.shape, C.shape)

        print(C)  # vertical stack

        D = np.hstack((A, B))  # horizontal stack

        print(D)
        # [1,1,1,2,2,2]

        print(A.shape, D.shape)
        # (3,) (6,)

        E = A[:, np.newaxis]
        print (E.shape)
        print (E)

        A = np.array([1, 1, 1])
        B = np.array([2, 2, 2])

        E = A[np.newaxis, :]
        print (E.shape)
        print (E)

        F = B[np.newaxis, :]
        print (F.shape)
        print (F)


        C = np.concatenate((E, F), axis=0)
        print (C.shape)
        print (C)

        print(np.split(C, 2, axis=0))

        print("==============")

        C = np.concatenate((E, F), axis=1)
        print (C.shape)
        print (C)
        print(np.split(C, 2, axis=1))

        A = np.arange(12).reshape((3, 4))
        print(A)

        print(np.split(A, 2, axis=1))
        print(np.split(A, 3, axis=0))

        "不等量的分割"

        print (A.shape)
        print (A)
        G=np.array_split(A, 3, axis=1)

        print (G)

        print(np.vsplit(A, 3))  # 等于 print(np.split(A, 3, axis=0))

        a = np.arange(4)
        a[0] = 11
        print(a)
        b = a.copy()  # deep copy
        print(b)  # array([11, 22, 33,  3])
        a[3] = 44
        print(a)  # array([11, 22, 33, 44])
        print(b)  # array([11, 22, 33,  3])

        import pandas as pd
        dates = pd.date_range('20130101', periods=6)
        df = pd.DataFrame(np.arange(24).reshape((6, 4)), index=dates, columns=['A', 'B', 'C', 'D'])

        print (df)
        x=df[df.A > 8]



        print (x)
        print (df.shape)
        for i in range(df.shape[0]):
            for j in range(df.shape[1]):
                df.iloc[i,j]=100;
        print (df)

        df['E'] = pd.Series([1, 2, 3, 4, 5, 6], index=pd.date_range('20130101', periods=6))
        print (df)
        dfnum = df.values
        df.to_pickle('df.pickle')

        print (df)

        import pandas as pd
        import numpy as np

        # 定义资料集
        df1 = pd.DataFrame(np.ones((3, 4)) * 0, columns=['a', 'b', 'c', 'd'])
        print (df1)
        df2 = pd.DataFrame(np.ones((3, 4)) * 1, columns=['a', 'b', 'c', 'd'])
        print (df2)
        df3 = pd.DataFrame(np.ones((3, 4)) * 2, columns=['a', 'b', 'c', 'd'])
        print (df3)

        # concat纵向合并
        res = pd.concat([df1, df2, df3], axis=0, ignore_index=True)

        # 打印结果
        print(res)

        res = pd.concat([df1, df2, df3], axis=1, ignore_index=True)
        print(res)

        # 定义资料集
        df1 = pd.DataFrame(np.ones((3, 4)) * 0, columns=['a', 'b', 'c', 'd'], index=[1, 2, 3])
        print (df1)
        df2 = pd.DataFrame(np.ones((3, 4)) * 1, columns=['b', 'c', 'd', 'e'], index=[2, 3, 4])
        print (df2)
        # 纵向"外"合并df1与df2
        res = pd.concat([df1, df2], axis=0, join='outer')

        print(res)

        # 纵向"内"合并df1与df2
        res = pd.concat([df1, df2], axis=0, join='inner')

        # 打印结果
        print(res)
        #     b    c    d
        # 1  0.0  0.0  0.0
        # 2  0.0  0.0  0.0
        # 3  0.0  0.0  0.0
        # 2  1.0  1.0  1.0
        # 3  1.0  1.0  1.0
        # 4  1.0  1.0  1.0

        # 重置index并打印结果
        res = pd.concat([df1, df2], axis=0, join='inner', ignore_index=True)
        print(res)

        # 定义资料集
        df1 = pd.DataFrame(np.ones((3, 4)) * 0, columns=['a', 'b', 'c', 'd'], index=[1, 2, 3])
        df2 = pd.DataFrame(np.ones((3, 4)) * 1, columns=['b', 'c', 'd', 'e'], index=[2, 3, 4])

        # 依照`df1.index`进行横向合并
        res = pd.concat([df1, df2], axis=1, join_axes=[df1.index])

        # 打印结果
        print(res)
        #     a    b    c    d    b    c    d    e
        # 1  0.0  0.0  0.0  0.0  NaN  NaN  NaN  NaN
        # 2  0.0  0.0  0.0  0.0  1.0  1.0  1.0  1.0
        # 3  0.0  0.0  0.0  0.0  1.0  1.0  1.0  1.0

        # 移除join_axes，并打印结果
        res = pd.concat([df1, df2], axis=1)
        print(res)
        #     a    b    c


        df1 = pd.DataFrame(np.ones((3, 4)) * 0, columns=['a', 'b', 'c', 'd'], index=[1, 2, 3])
        df2 = pd.DataFrame(np.ones((3, 4)) * 1, columns=['b', 'c', 'd', 'e'], index=[2, 3, 4])

        print ("==========================")
        dfnum1=df1.values;
        print (dfnum1);
        print (dfnum1.shape)

        dfnum2 = df2.values.astype(np.float32);

        print (dfnum2);
        print (dfnum2.shape)

        tmp=np.arange(100,103)
        print (tmp)
        print (tmp.shape)
        label=tmp.reshape((-1,1)).astype(np.float32)
        print (label.shape)
        print (label)


        dataTrain = np.hstack((dfnum1, label))  # so we can shuffle the order

        print (dataTrain)

        dataTrain = dataTrain.astype(np.float32)
        nSamples = dataTrain.shape[0]


        import torch


        x = torch.rand(15, 3)



        print ("====size shape==")

        print(x.size)
        print (x.shape)

        print("====size shape==")
        print (dataTrain.shape)
        print(dataTrain.size)

        a = torch.ones(5)
        print(a)

        b = a.numpy()
        print(b)

        a.add_(1)
        print(a)
        print(b)

        a = np.ones(5)
        b = torch.from_numpy(a)
        np.add(a, 1, out=a)
        print(a)
        print(b)

        from torch.autograd import Variable
        x = Variable(torch.ones(2, 2), requires_grad=True)
        print(x)

        y = x + 2
        print(y)

        print(y.grad_fn)



        print(dir(y))

        z = y * y * 3
        out = z.mean()

        print ("===z=")

        print(z)

        print ("===out=")

        print(out)

        print("====let’s backprop now out.backward() is equivalent to "
              "doing out.backward(torch.Tensor([1.0]))"
              "print gradients d(out)/dx")
        out.backward()

        print(x.grad)

        x = torch.randn(3)
        x = Variable(x, requires_grad=True)

        y = x * 2
        while y.data.norm() < 1000:
            y = y * 2

        print(y)

        gradients = torch.FloatTensor([0.1, 1.0, 0.0001])
        y.backward(gradients)

        print(x.grad)





        """
        from sklearn import datasets
        from sklearn.model_selection import train_test_split
        from sklearn.neighbors import KNeighborsClassifier
        iris = datasets.load_iris()
        iris_X = iris.data
        iris_y = iris.target

        print iris_X.shape
        print iris_y.shape

        # 标准化数据模块
        from sklearn import preprocessing
        import numpy as np

        # 将资料分割成train与test的模块
        from sklearn.model_selection import train_test_split

        # 生成适合做classification资料的模块
        from sklearn.datasets.samples_generator import make_classification

        # Support Vector Machine中的Support Vector Classifier
        from sklearn.svm import SVC

        # 可视化数据的模块
        import matplotlib.pyplot as plt

        # 生成具有2种属性的300笔数据
        X, y = make_classification(
            n_samples=300, n_features=2,
            n_redundant=0, n_informative=2,
            random_state=22, n_clusters_per_class=1,
            scale=100)

        # 可视化数据
        plt.scatter(X[:, 0], X[:, 1], c=y)
        plt.show()

        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
        clf = SVC()
        clf.fit(X_train, y_train)
        print(clf.score(X_test, y_test))
        # 0.477777777778
        X = preprocessing.scale(X)
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
        clf = SVC()
        clf.fit(X_train, y_train)
        print(clf.score(X_test, y_test))
        # 0.9

        from sklearn.datasets import load_iris  # iris数据集
        from sklearn.model_selection import train_test_split  # 分割数据模块
        from sklearn.neighbors import KNeighborsClassifier  # K最近邻(kNN，k-NearestNeighbor)分类算法

        # 加载iris数据集
        iris = load_iris()
        X = iris.data
        y = iris.target

        # 分割数据并
        X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=4)

        # 建立模型
        knn = KNeighborsClassifier()

        # 训练模型
        knn.fit(X_train, y_train)

        # 将准确率打印出
        print(knn.score(X_test, y_test))
        # 0.973684210526

        from sklearn.cross_validation import cross_val_score  # K折交叉验证模块

        # 使用K折交叉验证模块
        scores = cross_val_score(knn, X, y, cv=5, scoring='accuracy')

        # 将5次的预测准确率打印出
        print(scores)
        # [ 0.96666667  1.          0.93333333  0.96666667  1.        ]

        # 将5次的预测准确平均率打印出
        print(scores.mean())
        # 0.973333333333

        import matplotlib.pyplot as plt  # 可视化模块

        # 建立测试参数集
        k_range = range(1, 31)

        k_scores = []

        # 藉由迭代的方式来计算不同参数对模型的影响，并返回交叉验证后的平均准确率
        for k in k_range:
            knn = KNeighborsClassifier(n_neighbors=k)
            scores = cross_val_score(knn, X, y, cv=10, scoring='accuracy')
            k_scores.append(scores.mean())

        # 可视化数据
        plt.plot(k_range, k_scores)
        plt.xlabel('Value of K for KNN')
        plt.ylabel('Cross-Validated Accuracy')
        plt.show()

        import matplotlib.pyplot as plt
        k_range = range(1, 31)
        k_scores = []
        for k in k_range:
            knn = KNeighborsClassifier(n_neighbors=k)
            loss = -cross_val_score(knn, X, y, cv=10, scoring='mean_squared_error')
            k_scores.append(loss.mean())

        plt.plot(k_range, k_scores)
        plt.xlabel('Value of K for KNN')
        plt.ylabel('Cross-Validated MSE')
        plt.show()




        from sklearn.learning_curve import learning_curve  # 学习曲线模块
        from sklearn.datasets import load_digits  # digits数据集
        from sklearn.svm import SVC  # Support Vector Classifier
        import matplotlib.pyplot as plt  # 可视化模块
        import numpy as np
        digits = load_digits()
        X = digits.data
        y = digits.target

        train_sizes, train_loss, test_loss = learning_curve(
            SVC(gamma=0.001), X, y, cv=10, scoring='mean_squared_error',
            train_sizes=[0.1, 0.25, 0.5, 0.75, 1])

        # 平均每一轮所得到的平均方差(共5轮，分别为样本10%、25%、50%、75%、100%)
        train_loss_mean = -np.mean(train_loss, axis=1)
        test_loss_mean = -np.mean(test_loss, axis=1)

        plt.plot(train_sizes, train_loss_mean, 'o-', color="r",
                 label="Training")
        plt.plot(train_sizes, test_loss_mean, 'o-', color="g",
                 label="Cross-validation")

        plt.xlabel("Training examples")
        plt.ylabel("Loss")
        plt.legend(loc="best")
        plt.show()

        print ""

        from sklearn.learning_curve import validation_curve  # validation_curve模块
        from sklearn.datasets import load_digits
        from sklearn.svm import SVC
        import matplotlib.pyplot as plt
        import numpy as np

        # digits数据集
        digits = load_digits()
        X = digits.data
        y = digits.target

        # 建立参数测试集
        param_range = np.logspace(-6, -2.3, 5)

        # 使用validation_curve快速找出参数对模型的影响
        train_loss, test_loss = validation_curve(
            SVC(), X, y, param_name='gamma', param_range=param_range, cv=10, scoring='mean_squared_error')

        # 平均每一轮的平均方差
        train_loss_mean = -np.mean(train_loss, axis=1)
        test_loss_mean = -np.mean(test_loss, axis=1)

        # 可视化图形
        plt.plot(param_range, train_loss_mean, 'o-', color="r",
                 label="Training")
        plt.plot(param_range, test_loss_mean, 'o-', color="g",
                 label="Cross-validation")

        plt.xlabel("gamma")
        plt.ylabel("Loss")
        plt.legend(loc="best")
        plt.show()

        from sklearn import svm
        from sklearn import datasets

        clf = svm.SVC()
        iris = datasets.load_iris()
        X, y = iris.data, iris.target
        clf.fit(X, y)
        import pickle  # pickle模块

        # 保存Model(注:save文件夹要预先建立，否则会报错)
        with open('save/clf.pickle', 'wb') as f:
            pickle.dump(clf, f)

        # 读取Model
        with open('save/clf.pickle', 'rb') as f:
            clf2 = pickle.load(f)
            # 测试读取后的Model
            print(clf2.predict(X[0:1]))

            # [0]
        """

def run():
    io = IOBridge()
    io.SensorMLStepByStep()
    # io.FirstMLStepByStep()
    # io.npVSpdVSTorch()



if __name__ == '__main__':
    run()
    exit(0)

