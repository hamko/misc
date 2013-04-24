#include <stdio.h>

extern int yydebug;

void yyerror(const char* msg)
{
	fprintf(stderr, "%s", msg);
}

extern float g_l_value[3];
extern float g_psi_value[3];
int main(int argc, char **argv)
{
//    yydebug = 1;
    yydebug = 0;
	yyparse();

    int i;
    printf("l: ");
    for (i = 0; i < 3; i++)
        printf("%f ", g_l_value[i]);
    printf("\n");

    printf("psi: ");
    for (i = 0; i < 3; i++)
        printf("%f ", g_psi_value[i]);
    printf("\n");
}
