#!/usr/bin/env python
# -*-coding:utf-8-*-

"""

Implements
"""



import math


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

        #url = "https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"

        import csv

        """
        a = []
        pattern = {'huangy-incar01': 1001, 'huangy-pantpocket': 1002, 'UNKNOW': 1003, 'zhj_incar_packet': 1004,
                   'zhj_incar_hands': 1005, 'Stillontable': 1006}
        with open('/home/haijunz/133/Sensors-data-process-my-adding/20171226.csv', 'rb') as f:  # 采用b的方式处理可以省去很多问题
            reader = csv.reader(f)
            for row in reader:
                if (len(row) > 10):
                    a.append(row)



        with open('/home/haijunz/133/Sensors-data-process-my-adding/20171226-clean.csv', 'wb') as csvfile:
            writer = csv.writer(csvfile)
            for row in a:
                writer.writerow(row)
        """

        url = "/home/haijunz/133/Sensors-data-process-my-adding/20171226-clean.csv"

        names = ['timestamp', 't', 'x', 'y', 'z', 't1', 'x1', 'y1', 'z1', 't2', 'x2', 'y2', 'z2', 'class']

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
        dataset.plot(kind='box', subplots=True, layout=(4, 4), sharex=False, sharey=False)
        plt.show()


        # histograms
        dataset.hist()
        plt.show()

        # scatter plot matrix
        #scatter_matrix(dataset)
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



    def PdCategoricalData():
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



if __name__ == '__main__':
   io = IOBridge()
   io.SensorMLStepByStep()
   #io.FirstMLStepByStep()
   exit(0)

