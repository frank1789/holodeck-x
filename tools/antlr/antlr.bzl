load("@rules_cc//cc:defs.bzl", "cc_library")

def antlr_cc_library(name, srcs, linstener = True, visitor = False, visibility = None):
    """Generate C++ code from ANTLR .g4 grammar and wraps it in a cc_library

    Args:
        name: A unique name for the target library.
        srcs: A list containing your .g4 files.
        listener: Enable/Disable generate parse tree listener (default: enabled)
        visitor: Enable/Disable visitor generation (default: disabled)
        visibility: Target visibility settings.
    """

    if len(srcs) != 2 or len(srcs) == 1:
        fail("antlr_cc_library requires both a lexer and a parser .g4 file in 'srcs'")

    lexer = [f for f in srcs if "lexer" in f.lower()][0]
    parser = [f for f in srcs if "lexer" not in f.lower()][0]

    if not lexer or not parser:
        fail("antlr_cc_library requires both a lexer and a parser .g4 file in 'srcs'")

    # path cleanup
    # starlark does not have support for os.path
    lexer_filename = lexer.rpartition("/")[-1]
    parser_filename = parser.rpartition("/")[-1]

    # Strip the .g4 extension
    lexer_raw_name = lexer_filename.removesuffix(".g4")
    parser_raw_name = parser_filename.removesuffix(".g4")

    if lexer_raw_name.lower().endswith("lexer"):
        # e.g., "fbdLexer" -> "fbd"
        lexer_base_name = lexer_raw_name[:-5]
    else:
        lexer_base_name = lexer_raw_name

    if parser_raw_name.lower().endswith("parser"):
        # e.g., "fbdParser" -> "fbd"
        parser_base_name = parser_raw_name[:-6]
    else:
        parser_base_name = parser_raw_name

    # Explicitly list the artifacts that will exist flatly at $(RULEDIR)
    codegen_outs = [
        "{}Lexer.h".format(lexer_base_name),
        "{}Lexer.cpp".format(lexer_base_name),
       # "{}.tokens".format(lexer_raw_name),
        "{}Lexer.tokens".format(lexer_base_name),
        "{}Parser.h".format(parser_base_name),
        "{}Parser.cpp".format(parser_base_name),
        "{}BaseListener.h".format(parser_raw_name),
        "{}BaseListener.cpp".format(parser_raw_name),
        "{}Listener.h".format(parser_raw_name),
        "{}Listener.cpp".format(parser_raw_name),
    ]
    for c in codegen_outs:
        print("==>", c)

    codegen_target = "_{}_codegen".format(name)

    # TWO-STEP COMMAND PATH: Separated by && to guarantee correct file build order
    # inside the sandbox environment.
    cmd_str = (
        "java -jar $(location @antlr4_jar//file) -Dlanguage=Cpp -Xexact-output-dir -o $(RULEDIR) $(location {}) && " +
        "java -jar $(location @antlr4_jar//file) -Dlanguage=Cpp -Xexact-output-dir -o $(RULEDIR) -lib $(RULEDIR) $(location {})"
    ).format(lexer_filename, parser_filename)
    print("==>", cmd_str)

    native.genrule(
        name = codegen_target,
        srcs = srcs,
        outs = codegen_outs,
        cmd = cmd_str,
        tools = ["@antlr4_jar//file"],
    )

    cc_library(
        name = name,
        srcs = [":" + codegen_target],
        hdrs = [":" + codegen_target],
        includes = ["."],
       # copts = ["-I$(RULEDIR)"],
        deps = [
            "@antlr4-cpp-runtime//:antlr4-cpp-runtime",
        ],
        visibility = visibility,
    )
