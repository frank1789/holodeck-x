// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

lexer grammar fbdLexer;

/* Keywords must be at the top of the lexer */

BANG
    : '!'
    ;

POINT_CMD
    : 'PNT'
    | 'pnt'
    ;

LINE_CMD
    : 'LINE'
    | 'line'
    ;

SHAPE_CMD
    : 'SHPE'
    | 'shpe'
    ;

PLANE_SHAPE_OPTION
    : 'pln'
    ;

CYLINDER_SHAPE_OPTION
    : 'cyl'
    ;

CONE_SHAPE_OPTION
    : 'con'
    ;

TORUS_SHAPE_OPTION
    : 'tor'
    ;

SPHERE_SHAPE_OPTION
    : 'sph'
    ;

SEQ
    : 'SEQ'
    | 'seq'
    ;

/* specialized text grabbers */
COMMENT
    : '#' ~[\r\n]*
    ;

NEWLINE
    : '\n'
    | '\r\n'
    ;

WHITESPACE
    : [ \t]+ -> channel(HIDDEN)
    ;

NUMBER
    : [+-]? (DIGIT+ ('.' DIGIT*)? EXPONENT? | '.' DIGIT+ EXPONENT?)
    ;

// Fragments (Internal helper rules - cannot be matched as standalone tokens)
fragment EXPONENT
    : [eE] [+-]? DIGIT+
    ;

fragment DIGIT
    : [0-9]
    ;

NAME
    : [a-zA-Z0-9\-_]+
    ;

TEXT
    : [a-zA-Z]+
    ;
