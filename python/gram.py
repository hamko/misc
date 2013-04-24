def half(i):
    return i / 2

a = 10

for i in range(a):
    if i < a / 4:
        print "1 / 3 (" + str(i) + ")"
    elif i < half(a):
        print i
    else:
        print "Oh...Big..."
