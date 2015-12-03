# coding: UTF-8
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
import seaborn as sns

# おまじない
fig, ax = sns.plt.subplots()

# メインプロット位置変更
# Matplotlibでは、1つのメインプロットと複数のWidgetsで表現される図を書く
sns.plt.subplots_adjust(left=0.25, bottom=0.25)

# メインプロット
l = sns.heatmap(np.random.rand(10, 12), ax=ax)

# スライダー2つ
axcolor = 'lightgoldenrodyellow'
axfreq = sns.plt.axes([0.25, 0.1, 0.65, 0.03], axisbg=axcolor)
axamp = sns.plt.axes([0.25, 0.15, 0.65, 0.03], axisbg=axcolor)
sfreq = Slider(axfreq, 'Freq', 0.1, 30.0, valinit=3)
samp = Slider(axamp, 'Amp', 0.1, 10.0, valinit=5)
def update(val): # 今変更されたスライダーの値
    l = sns.heatmap(np.random.rand(10, 12), ax=ax, cbar=False)
    fig.canvas.draw_idle() # 更新
sfreq.on_changed(update)
samp.on_changed(update)


# ボタン
resetax = sns.plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', color=axcolor, hovercolor='0.975')
def reset(event): # eventって何だろう？クリックされたか離れたかかな？
    sfreq.reset()
    samp.reset()
button.on_clicked(reset)

# ラジオボタン
rax = sns.plt.axes([0.025, 0.5, 0.15, 0.15], axisbg=axcolor)
radio = RadioButtons(rax, ('red', 'blue', 'green'), active=0)
def colorfunc(label):
    l.set_color(label)
    fig.canvas.draw_idle()
radio.on_clicked(colorfunc)

sns.plt.show()
