import numpy as np

'''
def getIndex(indice, data_num):
    index = 0
    for i in range(len(data_num)):
        index += indice[i] * data_num[i]
    return index

def at(ndarr, indice):
    indice_num = ndarr.shape
    
    data_num = np.ones(ndarr.ndim)
    for vi in range(len(indice_num)-1):
        for ni in range(len(data_num)-vi-1):
            data_num[ni] *= indice_num[len(indice_num)-vi-1]
    print indice_num
    print data_num
    
    return ndarr.reshape(ndarr.size)[getIndex(indice, data_num)];

'''
ndarr = np.array([[1., 2., 3.], [4., 5., 6.], [7., 8., 9.]])
print ndarr(tuple([1,1,1])) # 5

