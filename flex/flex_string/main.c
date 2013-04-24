#include <stdio.h>

extern int yylex(void);
extern char *yytext;

int main(int argc, char **argv) 
{
    char str[1024] = "a b c\n123 asdaf 345\n";
    yy_scan_string(str);

    int return_code;
    while (return_code = yylex()) 
        if (return_code != '\n')
            printf("%s %d\n", yytext, return_code);

    return 0;
}
