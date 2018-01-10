
import pandas as pd
import numpy as np
from pandas import Series,DataFrame

import matplotlib.pyplot as plt


data_train = pd.read_csv("train.csv")
data_train.columns
data_train[data_train.Cabin.notnull()]['Survived'].value_counts()

g=data_train.groupby(['Cabin']).sum()
g.plot(kind='kde')
g.plot(kind='bar')
plt.show()




