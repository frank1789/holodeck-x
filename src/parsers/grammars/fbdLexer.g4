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
    : 'pln' | 'PLN'
    ;

CYLINDER_SHAPE_OPTION
    : 'cyl' | 'CYL'
    ;

CONE_SHAPE_OPTION
    : 'con' | 'CON'
    ;

TORUS_SHAPE_OPTION
    : 'tor' | 'TOR'
    ;

SPHERE_SHAPE_OPTION
    : 'sph' | 'SPH'
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
