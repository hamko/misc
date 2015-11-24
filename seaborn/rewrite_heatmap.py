# coding: UTF-8
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.widgets import Slider, Button, RadioButtons

fig, ax = sns.plt.subplots()

sns.heatmap(np.random.rand(10, 12), ax=ax)

resetax = sns.plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')
def reset(event):
    sns.heatmap(np.random.rand(10, 12)*1000, ax=ax, cmap = sns.diverging_palette(220, 10, as_cmap=True))
button.on_clicked(reset)

sns.plt.show()
