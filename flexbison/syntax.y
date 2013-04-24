%{

#define YYSTYPE char *
#include <stdio.h>
#include <stdlib.h>

float	g_l_value[3];
float	g_psi_value[3];

float	g_values[1000];
int		g_index = 0;

void array_copy(float* p)
{
	int i;
//    fprintf(stderr,"g_index: %d\n", g_index);
	for (i = 0; i < g_index; i++) {
		p[i] = g_values[i];
//        fprintf(stderr, "p[i] = %f\n", p[i]);
	}
}

%}

%token T_L T_PSI T_VALUE

%%

file
: /* empty */
| line file
;

line
: T_L ':' { g_index = 0; } float_list '\n'	{ array_copy(g_l_value); }
| T_PSI ':' { g_index = 0; } float_list '\n' { array_copy(g_psi_value); }
| '\n'
;

float_list
: /* empty */
| T_VALUE float_list {
	g_values[g_index++] = atof($1);
}
;
