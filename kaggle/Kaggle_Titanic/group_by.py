
import pandas as pd
import numpy as np
from pandas import Series,DataFrame

import matplotlib.pyplot as plt

#pandas.DataFrame.plot
#pandas.core.groupby.DataFrameGroupBy


#>>> data.groupby(func, axis=0).mean()
#>>> data.groupby(['col1', 'col2'])['col3'].mean()
#by : mapping, function, str, or iterable
#https://pandas.pydata.org/pandas-docs/stable/visualization.html#histograms

#http://pandas.pydata.org/pandas-docs/stable/groupby.html





#https://www.tutorialspoint.com/python_pandas/python_pandas_groupby.htm


import pandas as pd

ipl_data = {'Team': ['Riders', 'Riders', 'Devils', 'Devils', 'Kings',
          'kings', 'Kings', 'Kings', 'Riders', 'Royals', 'Royals', 'Riders'],
          'Rank': [1, 2, 2, 3, 3,4 ,1 ,1,2 , 4,1,2],
          'Year': [2014,2015,2014,2015,2014,2015,2016,2017,2016,2014,2015,2017],
          'Points':[876,789,863,673,741,812,756,788,694,701,804,690]}
df = pd.DataFrame(ipl_data)

print df.groupby('Team')



print df.groupby('Team').groups


print df.groupby(['Team','Year']).groups



grouped = df.groupby('Year')


print (type(grouped))



print grouped.describe




for name,group in grouped:
    print name
    print group
    print (type(group))


print grouped.get_group(2014)

print grouped['Points'].agg(np.mean)



grouped = df.groupby('Team')
print grouped.agg(np.size)


print grouped['Points'].agg([np.sum, np.mean, np.std])



score = lambda x: (x - x.mean()) / x.std()*10
print grouped.transform(score)


print df.groupby('Team').filter(lambda x: len(x) >= 3)




















data_train = pd.read_csv("train.csv")
data_train.columns
data_train[data_train.Cabin.notnull()]['Survived'].value_counts()
print data_train.groupby('Survived').groups



for name,group in data_train.groupby('Survived'):
    print name
    print group



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




