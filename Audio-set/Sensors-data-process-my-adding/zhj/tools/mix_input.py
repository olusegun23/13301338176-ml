import keras
import tensorflow as tf
import sys
import sklearn as sk
import pandas as pd
import numpy as np

from sklearn import preprocessing
# Encode text values to dummy variables(i.e. [1,0,0],[0,1,0],[0,0,1] for red,green,blue)








"""
https://www.heatonresearch.com/2017/07/22/keras-getting-started.html

https://github.com/jeffheaton/t81_558_deep_learning.git
https://github.com/projectlouis/mlbenchmark.git
"""


def encode_text_dummy(df, name):
    dummies = pd.get_dummies(df[name])
    for x in dummies.columns:
        dummy_name = "{}-{}".format(name, x)
        df[dummy_name] = dummies[x]
    df.drop(name, axis=1, inplace=True)
# Encode text values to indexes(i.e. [1],[2],[3] for red,green,blue).
def encode_text_index(df, name):
    le = preprocessing.LabelEncoder()
    df[name] = le.fit_transform(df[name])
    return le.classes_
# Convert all missing values in the specified column to the median
def missing_median(df, name):
    med = df[name].median()
    df[name] = df[name].fillna(med)
# Convert a Pandas dataframe to the x,y inputs that TensorFlow needs
def to_xy(df, target):
    result = []
    for x in df.columns:
        if x != target:
            result.append(x)
    # find out the type of the target column.  Is it really this hard? :(
    target_type = df[target].dtypes
    target_type = target_type[0] if hasattr(target_type, '__iter__') else target_type
    # Encode to int for classification, float otherwise. TensorFlow likes 32 bits.
    if target_type in (np.int64, np.int32):
        # Classification
        dummies = pd.get_dummies(df[target])
        a=df.as_matrix(result).astype(np.float32)
        b=dummies.as_matrix().astype(np.float32)

        return df.as_matrix(result).astype(np.float32), dummies.as_matrix().astype(np.float32)
    else:
        # Regression
        return df.as_matrix(result).astype(np.float32), df.as_matrix([target]).astype(np.float32)



def SimpleRegression():
    from keras.models import Sequential
    from keras.layers.core import Dense, Activation
    import pandas as pd
    import io
    import requests
    import numpy as np
    from sklearn import metrics
    url = "auto-mpg.csv"
    df = pd.read_csv(url, na_values=['NA', '?'])
    #df = pd.read_csv(io.StringIO(requests.get(url).content.decode('utf-8')), na_values=['NA', '?'])
    cars = df['name']
    df.drop('name', 1, inplace=True)
    missing_median(df, 'horsepower')
    x, y = to_xy(df, "mpg")
    model = Sequential()
    model.add(Dense(10, input_dim=x.shape[1], activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    model.compile(loss='mean_squared_error', optimizer='adam')

    model.fit(x, y, verbose=2, epochs=100)




    pred = model.predict(x)
    # Measure RMSE error.  RMSE is common for regression.
    score = np.sqrt(metrics.mean_squared_error(pred, y))
    print("Final score (RMSE): {}".format(score))
    # Sample predictions
    for i in range(10):
        print("{}. Car name: {}, MPG: {}, predicted MPG: {}".format(i + 1, cars[i], y[i], pred[i]))



def RegressionEarlyStop():
    import pandas as pd
    import io
    import requests
    import numpy as np
    from sklearn import metrics
    from keras.models import Sequential
    from keras.layers.core import Dense, Activation
    from keras.callbacks import EarlyStopping
    from sklearn.model_selection import train_test_split
    # url = "https://raw.githubusercontent.com/jeffheaton/t81_558_deep_learning/master/data/auto-mpg.csv"
    # df = pd.read_csv(io.StringIO(requests.get(url).content.decode('utf-8')), na_values=['NA', '?'])

    url = "auto-mpg.csv"
    df = pd.read_csv(url, na_values=['NA', '?'])
    cars = df['name']
    df.drop('name', 1, inplace=True)
    missing_median(df, 'horsepower')
    x, y = to_xy(df, "mpg")
    # Split into train/test
    x_train, x_test, y_train, y_test = train_test_split(
        x, y, test_size=0.25, random_state=45)
    model = Sequential()
    model.add(Dense(10, input_dim=x.shape[1], kernel_initializer='normal', activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    model.compile(loss='mean_squared_error', optimizer='adam')
    monitor = EarlyStopping(monitor='val_loss', min_delta=1e-3, patience=5, verbose=1, mode='auto')
    model.fit(x, y, validation_data=(x_test, y_test), callbacks=[monitor], verbose=2, epochs=1000)

def ClassificationModelEarlyStop():
    import pandas as pd
    import io
    import requests
    import numpy as np
    from sklearn import metrics
    from keras.models import Sequential
    from keras.layers.core import Dense, Activation
    from keras.callbacks import EarlyStopping
    from sklearn.model_selection import train_test_split
    url = "https://raw.githubusercontent.com/jeffheaton/t81_558_deep_learning/master/data/iris.csv"
    df = pd.read_csv(io.StringIO(requests.get(url).content.decode('utf-8')), na_values=['NA', '?'])
    species = encode_text_index(df, "species")
    x, y = to_xy(df, "species")
    # Split into train/test
    x_train, x_test, y_train, y_test = train_test_split(
        x, y, test_size=0.25, random_state=45)
    model = Sequential()
    model.add(Dense(10, input_dim=x.shape[1], kernel_initializer='normal', activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    model.add(Dense(y.shape[1], activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='adam')
    monitor = EarlyStopping(monitor='val_loss', min_delta=1e-3, patience=5, verbose=1, mode='auto')
    model.fit(x, y, validation_data=(x_test, y_test), callbacks=[monitor], verbose=2, epochs=1000)




    # Show the predictions (raw, probability of each class.) Print out the raw predictions. Because there are 3 species of iris, there are 3 columns.  The number in each column is
    # the probability that the flower is that type of iris.
    np.set_printoptions(suppress=True)
    pred = model.predict(x_test)
    print(pred[0:10])

    # The to_xy function represented the input in the same way.  Each row has only 1.0 value because each row is only one type
    # of iris.  This is the training data, we KNOW what type of iris it is.  This is called one-hot encoding.  Only one value
    # is 1.0 (hot)
    print(y_test[0:10])

    from sklearn.metrics import log_loss
    # Using the predictions (pred) and the known 1-hot encodings (y_test) we can compute the log-loss error.
    # The lower a log loss the better.  The probabilities (pred) from the previous section specify how sure the neural network
    # is of its prediction.  Log loss error pubishes the neural network (with a lower score) for very confident, but wrong,
    # classifications.
    print(log_loss(y_test, pred))

    # Usually the column (pred) with the highest prediction is considered to be the prediction of the neural network.  It is easy
    # to convert the predictions to the expected iris species.  The argmax function finds the index of the maximum prediction
    # for each row.
    predict_classes = np.argmax(pred, axis=1)
    expected_classes = np.argmax(y_test, axis=1)
    print("Predictions: {}".format(predict_classes))
    print("Expected: {}".format(expected_classes))

    # Of course it is very easy to turn these indexes back into iris species.  We just use the species list that we created earlier.
    print(species[predict_classes[1:10]])







