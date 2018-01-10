
# Import all libraries needed for the tutorial
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys #only needed to determine Python version number
import matplotlib #only needed to determine Matplotlib version number



#df1=pd.DataFrame(np.random.randn(3,4),columns=['a','b','c','d'])

df1=pd.DataFrame([[1,1,1,1],[2,2,2,2],[3,3,3,3]],columns=['a','b','c','d'])

df2=pd.DataFrame([[1,1,1],[7,7,7]],columns=['a','e','f'])
#df2=pd.DataFrame(np.random.randn(2,3),columns=['a','e','f'])


print (df1)
print (df2)
df3=pd.concat([df1,df2])
print (df3)



df4=df1.join(df2)
print (df4)

