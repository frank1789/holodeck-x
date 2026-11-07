// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

parser grammar fbdParser;

options {
    tokenVocab = fbdLexer;
}

file
    : expression* EOF
    ;

emptyLine
    : NEWLINE
    ;

comment
    : COMMENT NEWLINE
    ;

// identifier of the element can be an alphanumeric string or a bang (!)
// when using a bang, the name wil be generate automatically by the program
identifierName
    : NAME
    | BANG
    ;

pointCommand
    : POINT_CMD id = identifierName x = NUMBER y = NUMBER z = NUMBER NEWLINE
    ;

value
    : NUMBER
    | NAME
    ;

lineCommand
    : LINE_CMD id = identifierName start_point = value end_point = value div = NUMBER NEWLINE
    | LINE_CMD id = identifierName start_point = value end_point = value cp = value div = NUMBER NEWLINE
    | LINE_CMD id = identifierName start_point = value end_point = value cp = value div = NUMBER bias = NUMBER NEWLINE
    | LINE_CMD id = identifierName start_point = value end_point = value cp = value SEQ bias = NUMBER NEWLINE
    ;

planeShape
    : PLANE_SHAPE_OPTION p1 = value p2 = value p3 = value
    ;

cylinderShape
    : CYLINDER_SHAPE_OPTION p1 = value p2 = value r1 = value
    ;

coneShape
    : CONE_SHAPE_OPTION p1 = value p2 = value r1 = value r2 = value
    ;

torusShape
    : TORUS_SHAPE_OPTION p1 = value r1 = value p2 = value r2 = value
    ;

sphereShape
    : SPHERE_SHAPE_OPTION p1 = value r1 = value
    ;

shapeCommand
    : SHAPE_CMD id = identifierName planeShape NEWLINE
    | SHAPE_CMD id = identifierName cylinderShape NEWLINE
    | SHAPE_CMD id = identifierName coneShape NEWLINE
    | SHAPE_CMD id = identifierName torusShape NEWLINE
    | SHAPE_CMD id = identifierName sphereShape NUMBER NEWLINE
    ;

expression
    : comment
    | pointCommand
    | lineCommand
    | shapeCommand
    | emptyLine
    ;
