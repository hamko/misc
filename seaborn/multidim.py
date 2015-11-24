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
sns.plt.subplot(2,1,1)
uniform_data = np.random.rand(10, 12)
ax = sns.heatmap(uniform_data)
sns.plt.subplot(2,1,2)
uniform_data = np.random.rand(10, 12)
ax = sns.heatmap(uniform_data)
sns.plt.show()

# スライダー2つ
axcolor = 'lightgoldenrodyellow'
axfreq = sns.plt.axes([0.25, 0.1, 0.65, 0.03], axisbg=axcolor)
axamp = sns.plt.axes([0.25, 0.15, 0.65, 0.03], axisbg=axcolor)
sfreq = Slider(axfreq, 'Freq', 0.1, 30.0, valinit=f0)
samp = Slider(axamp, 'Amp', 0.1, 10.0, valinit=a0)
def update(val): # valって何だろう？今変更されたスライダーの値かな？
    amp = samp.val # Slider.valで現在のスライダーの値が読める
    freq = sfreq.val
    l.set_ydata(amp*np.sin(2*np.pi*freq*t)) # 変更
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
