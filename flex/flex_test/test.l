%{
%}

ALPHA   [a-z]
DIGIT   [0-9]
SPACE   [ \t]
OPERATOR    [*]

%%

{SPACE}     return ' ';
\n          return '\n';
\r\n        return '\n';
{DIGIT}     return 1;
{OPERATOR}     return 1;

{ALPHA}+    return 2;
