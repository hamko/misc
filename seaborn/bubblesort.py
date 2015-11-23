def bubblesort(a):
    for i in range(len(a)-2):
        for j in range(len(a)-1-i):
            if a[j] < a[j+1]:
                tmp = a[j]
                a[j] = a[j+1]
                a[j+1] = tmp
    


a=[2,4,5,1,2]
bubblesort(a)
for i in range(len(a)):
    print a[i]
