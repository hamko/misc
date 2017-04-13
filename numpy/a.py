#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np

print(np.__version__)
np.show_config()

Z = np.zeros(10)
print(Z)

Z[4] = 1
print(Z)

Z = np.arange(50)
print(Z)
Z = Z[::-1]
print(Z)


Z = np.arange(9).reshape(3, 3)
print(Z)

Z = np.random.random((10, 10))
Zmin, Zmax, Zmean= Z.min(), Z.max(), Z.mean()
print(Zmin, Zmax, Zmean)

#スライスのマイナスは循環
#numpyの,はand
Z = np.ones((10, 10))
Z[1:-1,1:-1] = 0
print(Z)

# nanが入った演算はすべてnanかfalse
print(0 * np.nan)
print(np.nan == np.nan)
print(np.inf > np.nan)
print(np.nan - np.nan)

# false, 浮動小数点は気をつけて
print(0.3 == 3 * 0.1)
print(abs(0.3 - 3 * 0.1) < 0.001)

# dtypeで型指定できる
Z = np.zeros((8,8),dtype=int)
Z[1::2,::2] = 1
print(Z)
Z[::2,1::2] = 1
print(Z)

#一次元化も楽
print(np.unravel_index(100, (6, 7, 8)))

Z = np.dot(np.ones((5, 3)), np.ones((3, 2)))
print(Z)

#添字の中の&はandではだめ
Z = np.arange(11)
Z[(3 < Z) & (Z <= 8)] *= -1
print(Z)

#無名空間のsumは、-1からはじめて、という意味
#numpyのsumは、-1行目の、という意味
print(sum(range(5),-1))
from numpy import *
print(sum(range(5),-1))

Z = np.arange(10)
print(Z)
print(Z**Z) #OK, 要素ごとの累乗
print(3<<Z) #OK, 3*2^Z[i]
print(3<<Z>>2) #OK, (3*2^Z[i])/4
print(Z<-Z) #OK, -は要素ごとnegate
print(1j*Z) #OK, 複素数も扱える
print(Z/1/1)
#print(Z<Z>Z) #ダメ

Z = np.random.uniform(-10,+10,10) # [-10, +10]の一様分布を10個
print(Z)
# copysign(a, b): |a|*sgn(b)
# trunc: 小数点を除く切り捨て（floorは最小整数に合わせる）
print(np.trunc(Z + np.copysign(0.5, Z))) 


Z = np.random.uniform(0,10,10)
# 小数点にmodがある(1.4%1 = 0.4)
# astypeで一括キャストできる
print (Z - Z%1)
print (np.floor(Z))
print (np.ceil(Z)-1)
print (Z.astype(int))
print (np.trunc(Z))

#行列にベクトルを足すと、各行あるいは各列に足される
Z=np.zeros((5,5))
Z += np.arange(5)
print(Z)
Z=np.zeros((5,5))
Z += np.arange(5).reshape(5,1)
print(Z)

# ジェネレータでは、returnではなくyieldのみ使う
# ジェネレータでは、前回のyieldのプログラムポイントに戻って、yieldが来るまで進み、それを返す。
# np.fromiterのcountは、-1ですべてのデータを見るという意味になる
def generate():
    for x in range(10):
        yield x
Z = np.fromiter(generate(),dtype=float,count=-1)
print(Z)
Z = np.fromiter(generate(),dtype=float,count=3)
print(Z)

#linspaceで線形補間できる
#コーディングルールとして、[1:-1]は最初と最後を飛ばすという意味になる
Z = np.linspace(0,1,12,endpoint=True)[1:-1]
print(Z)
Z = np.linspace(0,1,12,endpoint=True)[1:-1]
print(Z)

#sort関数がある
Z = np.random.random(10)
Z.sort()
print(Z)

# reduceできる
Z = np.arange(10)
print(np.add.reduce(Z))

# vectorの等価性判定はallclose
# ==を使うと、各要素の判定になってしまう
A = np.random.randint(0,2,5)
B = np.random.randint(0,2,5)
equal = np.allclose(A,B)
print(A == B)
print(equal)

#ランタイムでconstにできる
Z = np.zeros(10)
Z.flags.writeable = False
#Z[0] = 2 # エラーが起きる
Z.flags.writeable = True

#代入、sqrtなど、大体の関数がmapっぽく動いている
Z = np.random.random((10, 2))
X, Y = Z[:, 0], Z[:, 1]
R = np.sqrt(X**2 + Y**2)
print(R)

#最大要素を取得できる
Z = np.random.random(10)
print(Z.argmax())
print(Z.max())

#行列にペアを突っ込むことができる（要するに3階テンソルみたくなっている。行列が2つある）
Z = np.zeros((5,5), [('x',float),('y',float)])
Z['x'], Z['y'] = np.meshgrid(np.linspace(0,4,5), np.linspace(0,4,5))
print(Z)

#C_{ij}をx_iとy_jから生成できる
#行列式はlinalg.det
X = np.arange(3)
Y = X+0.5
C = 1.0 / np.subtract.outer(X, Y)
print(C)
print(np.linalg.det(C))


# dtypeの情報が手に入る
for dtype in [np.int8, np.int32, np.int64]:
    print(np.iinfo(dtype).min)
    print(np.iinfo(dtype).max)
for dtype in [np.float32, np.float64]:
    print(np.finfo(dtype).min)
    print(np.finfo(dtype).max)
    print(np.finfo(dtype).eps)

#参照で型だけ変えることができる
Z = np.arange(10, dtype=np.int32)
Z = Z.astype(np.float32, copy=False)

#i番目をキーとしてソートする
Z = np.random.randint(0,10,(5,5))
print(Z)
print(Z[Z[:,1].argsort()])
