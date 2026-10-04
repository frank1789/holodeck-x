// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar resultfileParser;

options {
    tokenVocab = resultfileLexer;
}

file
    : (line)* EOF
    ;

line
    : (TAB | WHITESPACE)* NEWLINE
    | comment
    | node
    | user
    | model
    | date
    | time
    | host
    | program
    | version
    | directory
    | dbname
    | material
    | end
    ;

comment
    : '!'*? NEWLINE
    ;

start_node_block
    : (TAB | WHITESPACE)* END_CURRENT_BLOCK NEWLINE
    ;

start_element_block
    : (TAB | WHITESPACE)* START_ELEMENT_BLOCK NEWLINE
    ;

element
    : LINE_ID (TAB | WHITESPACE)*? ID (TAB | WHITESPACE)*? ELEMENT_TYPE
    | LINE_ID (TAB | WHITESPACE) DIGIT+
    ;

node
    : (TAB | WHITESPACE)* number WHITESPACE? number WHITESPACE? number WHITESPACE? number NEWLINE
    ;

user
    : (TAB | WHITESPACE)* USER WHITESPACE? NAME NEWLINE
    ;

model
    : (TAB | WHITESPACE)* MODEL_NAME WHITESPACE? NAME NEWLINE
    ;

date
    : (TAB | WHITESPACE)* DATE WHITESPACE? NAME NEWLINE
    ;

time
    : (TAB | WHITESPACE)* TIME WHITESPACE? NAME NEWLINE
    ;

host
    : (TAB | WHITESPACE)* HOST WHITESPACE? NAME NEWLINE
    ;

program
    : (TAB | WHITESPACE)* PROGRAM WHITESPACE? NAME NEWLINE
    ;

version
    : (TAB | WHITESPACE)* VERSION WHITESPACE? NAME NEWLINE
    ;

directory
    : (TAB | WHITESPACE)* DIRECTORY WHITESPACE? NAME NEWLINE
    ;

dbname
    : (TAB | WHITESPACE)* DBNAME WHITESPACE? NAME NEWLINE
    ;

material
    : (TAB | WHITESPACE)* MATERIAL WHITESPACE? NAME NEWLINE
    ;

end_current_block
    : (TAB | WHITESPACE)* END_CURRENT_BLOCK NEWLINE
    ;

end
    : (TAB | WHITESPACE)* END NEWLINE
    ;

number
    : SIGN? (FLOAT | INT)
    ;
