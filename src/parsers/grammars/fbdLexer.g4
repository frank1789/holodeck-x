// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

lexer grammar fbdLexer;

FLOAT
    : DIGIT+ '.' DIGIT* EXPONENT?
    | '.' DIGIT+ EXPONENT?
    | DIGIT+ EXPONENT
    ;

INT
    : DIGIT+
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

COMMENT
    : '#'
    ;

NEWLINE
    : '\n'
    | '\r'
    | '\r\n'
    ;

WHITESPACE
    : ' ' -> skip
    ;
