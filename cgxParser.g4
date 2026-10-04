// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar cgxParser;

options {
    tokenVocab = cgxLexer;
}

file
    : (line)* EOF
    ;

number
    : SIGN? (FLOAT | INT)
    ;

point
    : (WHITESPACE | TAB)? POINT (WHITESPACE | TAB)? NAME (WHITESPACE | TAB)? number (
        WHITESPACE
        | TAB
    )? number (WHITESPACE | TAB)? number NEWLINE
    ;

line
    : (WHITESPACE | TAB)? LINE (WHITESPACE | TAB)? NAME*? number NEWLINE
    ;

mesh
    : (WHITESPACE | TAB)? MESH (WHITESPACE | TAB)? ALL ELEMENT_TYPE NEWLINE
    ;

element_type
    : (WHITESPACE | TAB)? ELEMENT_TYPE ALL ELEMENT NEWLINE
    ;

cmd
    : point
    | line
    | mesh
    | element_type
;
