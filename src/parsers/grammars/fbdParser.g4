// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar fbdParser;

options {
    tokenVocab = fbdLexer;
}

file
    : (line)* EOF
    ;

line
    : expression NEWLINE
    ;

comment
    : COMMENT .*? NEWLINE
    ;

command
    : NEWLINE
    ;

expression
    : comment
    | command WHITESPACE*?
    ;
