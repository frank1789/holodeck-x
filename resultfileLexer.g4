// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

lexer grammar resultfileLexer;

USER_DATA
    : DIGIT 'U'
    ;

COMMENT
    : DIGIT 'C'
    ;

MODEL_NAME
    : USER_DATA
    ;

USER
    : USER_DATA 'USER'
    ;

DATE
    : USER_DATA 'DATE'
    ;

TIME
    : USER_DATA 'TIME'
    ;

HOST
    : USER_DATA 'HOST'
    ;

PROGRAM
    : USER_DATA 'PGM'
    ;

VERSION
    : USER_DATA 'VERSION'
    ;

DIRECTORY
    : USER_DATA 'DIR'
    ;

DBNAME
    : USER_DATA 'DBN'
    ;

MATERIAL
    : USER_DATA 'MAT'
    ;

END
    : '9999'
    ;

FLOAT
    : DIGIT+ '.' DIGIT* EXPONENT?
    | '.' DIGIT+ EXPONENT?
    | DIGIT+ EXPONENT
    ;

INT
    : DIGIT+ // Matches -12
    ;

SIGN
    : [+-]
    ;

// Fragments (Internal helper rules)
fragment EXPONENT
    : [eE] [+-]? DIGIT+
    ;

fragment DIGIT
    : [0-9]
    ;

NAME
    : [a-zA-Z0-9\-_]+
    ;

NEWLINE
    : ('\r'? '\n' | '\r')+
    ;

TAB
    : ('\t' | '        ' | '    ')
    ;

WHITESPACE
    : ' ' -> skip
    ;
