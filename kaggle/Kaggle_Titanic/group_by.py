
import pandas as pd
import numpy as np
from pandas import Series,DataFrame

import matplotlib.pyplot as plt

#pandas.DataFrame.plot





data_train = pd.read_csv("train.csv")
data_train.columns
data_train[data_train.Cabin.notnull()]['Survived'].value_counts()

sub=['Cabin','Survived']
data=data_train[sub]

g=data.groupby(['Cabin']).sum()

print ('================kde')

print (type(g))
print ('=========================kde')
g.plot(kind='kde')
g.plot(kind='bar')
g.plot(kind='line')
g.plot(kind='hist')
g.plot(kind='box')
g.plot(kind='area')
#g.plot(kind='pie')

#g.plot(kind='scatter')




plt.show()




