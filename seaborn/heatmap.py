import numpy as np; np.random.seed(0)
import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

uniform_data = np.random.rand(10, 12)
ax = sns.heatmap(uniform_data)
sns.plt.axis("off")
sns.plt.show()
