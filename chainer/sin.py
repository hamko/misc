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

def func_name():
    return "sqrt(abs(sin(exp(x)))"
def func(x):
    return np.sqrt(np.abs(np.sin(np.exp(x))))

# データ
def get_dataset(N):
    x = np.linspace(0, 2 * np.pi, N).astype(np.float32)
    y = func(x).astype(np.float32)
    return x, y

# ニューラルネットワーク
class MyChain(Chain):
    def __init__(self, n_units=10):
        super(MyChain, self).__init__(
             l1=L.Linear(1, n_units),
             l2=L.Linear(n_units, n_units),
             l3=L.Linear(n_units, 1))

    # call: var -> var
    def __call__(self, x):
        h1 = F.relu(self.l1(x))
        h2 = F.relu(self.l2(h1))
        h3 = self.l3(h2)
        return h3

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
    n_epoch = 200
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
            x_batch = Variable(x_train[perm[i:i + batchsize]].reshape(batchsize, 1)) # var
            y_batch = Variable(y_train[perm[i:i + batchsize]].reshape(batchsize, 1)) # var

            model.zerograds()
            loss = F.mean_squared_error(model(x_batch), y_batch) 
            sum_loss += loss.data * batchsize
            loss.backward()
            optimizer.update()

        average_loss = sum_loss / N
        train_losses.append(average_loss)

        # test
        loss = F.mean_squared_error(model(Variable(x_test.reshape(len(x_test), 1))), Variable(y_test.reshape(len(y_test), 1))) # loss = var(1)
        test_losses.append(loss.data) # arr

        # 学習過程を出力
        if epoch % 10 == 0:
            print("epoch: {}/{} train loss: {} test loss: {}".format(epoch, n_epoch, average_loss, loss.data))

        # 学習結果のグラフ作成
        if epoch in [10, n_epoch]:
            theta = np.linspace(0, 2 * np.pi, N_test).astype(np.float32) # np
            real = func(theta) # np
#            test = model.get_predata(theta) # np
            test = model(Variable(theta.reshape(len(theta), 1))).data # np
            plt.plot(theta, real, label = func_name())
            plt.plot(theta, test, label = "test")
            plt.legend()
            plt.grid(True)
#            plt.xlim(0, 2 * np.pi)
#            plt.ylim(-1.2, 1.2)
            plt.title(func_name)
            plt.xlabel("theta")
            plt.ylabel("amp")
            plt.savefig("fig/fig_"+func_name()+"_epoch{}.png".format(epoch)) # figフォルダが存在していることを前提
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
