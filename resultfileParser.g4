// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar resultfileGrammar;

options {
    tokenVocab = resultfileLexer;
}

file
    : (line | comment)* EOF
    ;

line
    : (TAB | WHITESPACE)* NEWLINE
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
    : COMMENT .*? NEWLINE
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

end
    : (TAB | WHITESPACE)* END NEWLINE
    ;

number
    : FLOAT
    | INT
    ;
