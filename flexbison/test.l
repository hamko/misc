%{
#include "y.tab.h"
%}

DIGIT	[0-9]
NAME	[a-zA-Z_][a-zA-Z0-9]*

%%

[ \t]

#.*

"l"		return T_L;
"psi"	return T_PSI;

{DIGIT}+	yylval = (int)strdup(yytext); return T_VALUE;
\r\n	return '\n';
\n		return '\n';
.		return yytext[0];
