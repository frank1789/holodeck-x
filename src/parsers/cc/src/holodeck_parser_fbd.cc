#include "holodeck_parser_fbd.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "antlr4-runtime.h"
#include "fbdLexer.h"
#include "fbdParser.h"
#include "fbdParserBaseListener.h"

bool isNumber(const std::string& str) {
  if (str.empty()) return false;

  try {
    size_t processedChars = 0;
    std::stod(str, &processedChars);

    // Ensure the ENTIRE string was a valid number, not just the front part
    // (e.g. "10.5abc")
    return processedChars == str.size();
  } catch (const std::invalid_argument&) {
    return false;  // Not a number at all
  } catch (const std::out_of_range&) {
    return true;  // It IS a number, it's just too massive for a C++ double!
  }
}

bool isAlphanumeric(const std::string& str) {
  if (str.empty()) return false;

  return std::all_of(str.begin(), str.end(), [](unsigned char c) {
    return std::isalnum(c) ||
           c == '_';  // Permits characters, digits, and underscores
  });
}

namespace holodeckx {

class FBDParserListener : public fbdParserBaseListener {
 public:
  FBDParserListener() = default;

  // auto exitCommand(fbdParser::CommandContext* ctx) -> void override {
  //   // Handle the parsed file context here
  // }

  auto exitPointCommand(fbdParser::PointCommandContext* ctx) -> void override {
    // const auto cmd = ctx->POINT_CMD()->getText();
    const auto id = ctx->id->getText();
    const auto x = (ctx->x->getText());
    const auto y = (ctx->y->getText());
    const auto z = (ctx->z->getText());

    // if (isNumber(radiusText)) {
    //     // Direct value execution
    //     double radiusVal = std::stod(radiusText);
    //     std::cout << "Parsed static float radius: " << radiusVal << "\n";
    // }
    // else if (isAlphanumeric(radiusText)) {
    //     // Variable database lookup execution
    //     std::string variableName = radiusText;
    //     std::cout << "Looking up database variable named: " << variableName
    //     << "\n";
    //     // double radiusVal = myVariableDatabase.get(variableName);
    // }
    // else {
    //     std::cerr << "Syntax Error: Parameter is neither a valid number nor
    //     variable name.\n";
    // }

    std::cout << "found: "
              // << "cmd: " << cmd << "\n"
              << "id: " << id << " " << std::fixed << "x: " << x << " "
              << "y: " << y << " "
              << "z: " << z << "\n";
  }

  auto exitComment(fbdParser::CommentContext* ctx) -> void override {
    // do something with the comment context
    // std::cout << ctx->getText() << std::endl;
  }
};

ParserFBD::ParserFBD(std::string filename) : filename_(std::move(filename)) {}

void ParserFBD::Parse() {
  std::ifstream input_file(filename_, std::ios::in | std::ios::binary);
  if (!input_file.is_open()) {
    const auto error_message = "failed to open file";
    // std::format("Failed to open file: {}", filename_);
    throw std::runtime_error(error_message);
  }

  antlr4::ANTLRInputStream input(input_file);
  fbdLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  fbdParser parser(&tokens);
  fbdParser::FileContext* tree = parser.file();
  //
  FBDParserListener listener;
  antlr4::tree::ParseTreeWalker walker;
  walker.walk(&listener, tree);
  // ImageVisitor visitor;
  // Scene scene = std::any_cast<Scene>(visitor.visitFile(tree));
  // scene.draw();
  input_file.close();
}

}  // namespace holodeckx
