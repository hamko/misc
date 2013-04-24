#include <stdio.h>

int main(void)
{
    // a[4]!!!
    int a[4];
    a[4] = 0;
    printf("a[4] is %d\n", a[4]);


    // forgot to fclose!!!
    FILE *fp = fopen("test.txt", "r"); // test.txt absolutely exists!
    char tmp[1024]; fgets(tmp, sizeof(tmp), fp);
    printf("test.txt: %s", tmp);
    /* fclose(fp); */
   

    // mistook to initialize!!!
    // これみたいなことを実際にやって、90分くらい悩んで分からなかったのが、
    // Vargrind使ったら5分でデバッグできた。
    // ちなみにこれは、b[i]=iの配列を作ろうとしているところ、iをMAXNUMと書き間違えた感じ。
#define MAXNUM 5
    int b[MAXNUM];
    int i;
    for (i = 0; i < MAXNUM; i++) 
        b[MAXNUM] = i;
//      b[i] = i;
    for (i = 0; i < MAXNUM; i++) 
        printf("b[%d] = %d\n", i, b[i]);

    return 0;
}
