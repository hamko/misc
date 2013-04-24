#include <stdio.h>

extern int yylex(void);
extern char *yytext;

int main(int argc, char **argv) 
{
    int return_code;
    while (return_code = yylex()) 
        if (return_code != '\n')
            printf("%s %d\n", yytext, return_code);

    return 0;
}
