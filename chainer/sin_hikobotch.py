# -*- coding: utf-8 -*-

# とりあえず片っ端からimport
import numpy as np
import chainer
from chainer import cuda, Function, gradient_check, Variable, optimizers, serializers, utils
from chainer import Link, Chain, ChainList
import chainer.functions as F
import chainer.links as L
import time
from matplotlib import pyplot as plt

# データ
def get_dataset(N):
    x = np.linspace(0, 2 * np.pi, N)
    y = np.sin(x)
    return x, y

# ニューラルネットワーク
class MyChain(Chain):
    def __init__(self, n_units=10):
        super(MyChain, self).__init__(
             l1=L.Linear(1, n_units),
             l2=L.Linear(n_units, n_units),
             l3=L.Linear(n_units, 1))

    # call: (np \times np) -> var
    def __call__(self, x_data, y_data):
        x = Variable(x_data.astype(np.float32).reshape(len(x_data),1)) # Variableオブジェクトに変換
        y = Variable(y_data.astype(np.float32).reshape(len(y_data),1)) # Variableオブジェクトに変換
        return F.mean_squared_error(self.predict(x), y)

    # predict: var -> var
    def predict(self, x):
        h1 = F.relu(self.l1(x))
        h2 = F.relu(self.l2(h1))
        h3 = self.l3(h2)
        return h3

    # predict: np -> np
    def get_predata(self, x):
        return self.predict(Variable(x.astype(np.float32).reshape(len(x),1))).data

# main
if __name__ == "__main__":

    # 学習データ
    N = 1000
    x_train, y_train = get_dataset(N) # np

    # テストデータ
    N_test = 900
    x_test, y_test = get_dataset(N_test) # np

    # 学習パラメータ
    batchsize = 10
    n_epoch = 100
    n_units = 50

    # モデル作成
    model = MyChain(n_units) # model: var -> var
    optimizer = optimizers.Adam()
    optimizer.setup(model)

    # 学習ループ
    train_losses =[] # arr
    test_losses =[] # arr
    print("start...")
    start_time = time.time()
    for epoch in range(1, n_epoch + 1):

        # training
        perm = np.random.permutation(N) # id
        sum_loss = 0
        for i in range(0, N, batchsize):
            # 訓練データからN個ランダムにとってきてバッチとする
            x_batch = x_train[perm[i:i + batchsize]] # np
            y_batch = y_train[perm[i:i + batchsize]] # np

            model.zerograds()
            loss = model(x_batch,y_batch)
            sum_loss += loss.data * batchsize
            loss.backward()
            optimizer.update()

        average_loss = sum_loss / N
        train_losses.append(average_loss)

        # test
        loss = model(x_test,y_test) # loss = var(1)
        test_losses.append(loss.data) # arr

        # 学習過程を出力
        if epoch % 10 == 0:
            print("epoch: {}/{} train loss: {} test loss: {}".format(epoch, n_epoch, average_loss, loss.data))

        # 学習結果のグラフ作成
        if epoch in [10, n_epoch]:
            theta = np.linspace(0, 2 * np.pi, N_test) # np
            sin = np.sin(theta) # np
            test = model.get_predata(theta) # np
            plt.plot(theta, sin, label = "sin")
            plt.plot(theta, test, label = "test")
            plt.legend()
            plt.grid(True)
            plt.xlim(0, 2 * np.pi)
            plt.ylim(-1.2, 1.2)
            plt.title("sin")
            plt.xlabel("theta")
            plt.ylabel("amp")
            plt.savefig("fig/fig_sin_epoch{}.png".format(epoch)) # figフォルダが存在していることを前提
            plt.clf()

    print("end")

    interval = int(time.time() - start_time)
    print("実行時間: {}sec".format(interval))


    # 誤差のグラフ作成
    plt.plot(train_losses, label = "train_loss")
    plt.plot(test_losses, label = "test_loss")
    plt.yscale('log')
    plt.legend()
    plt.grid(True)
    plt.title("loss")
    plt.xlabel("epoch")
    plt.ylabel("loss")
    plt.savefig("fig/fig_loss.png") # figフォルダが存在していることを前提
    plt.clf()
