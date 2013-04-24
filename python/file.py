f = open("input", "r")
out = open("output", "w")

for row in f:
    tok = row.split() #空白文字でsplit
    print tok
    for i in tok:
        out.write(i)
        out.write('\n')

f.close();
out.close();

