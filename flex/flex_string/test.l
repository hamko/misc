%{
%}

ALPHA   [a-z]
DIGIT   [0-9]
SPACE   [ \t]

%%

{SPACE}     return ' ';
\n          return '\n';
\r\n        return '\n';
{DIGIT}     return 1;

{ALPHA}+    return 2;
