import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt

import seaborn as sns
sns.set(style="whitegrid", color_codes=True)

np.random.seed(sum(map(ord, "categorical")))

titanic = sns.load_dataset("titanic")
tips = sns.load_dataset("tips")
iris = sns.load_dataset("iris")

sns.barplot(x="sex", y="survived", hue="class", data=titanic);
sns.countplot(x="deck", data=titanic, palette="Greens_d");

plt.show()
