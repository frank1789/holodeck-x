#include "holodeck_parser_fbd.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "antlr4-runtime.h"
#include "fbdLexer.h"
#include "fbdParser.h"
#include "fbdParserBaseListener.h"

namespace holodeckx {

class FBDParserListener : public fbdParserBaseListener {
 public:
  FBDParserListener() = default;

  // auto exitCommand(fbdParser::CommandContext* ctx) -> void override {
  //   // Handle the parsed file context here
  // }

  auto exitPointCommand(fbdParser::PointCommandContext *ctx) -> void override {
    const auto cmd = ctx->POINT_CMD()->getText();
    const auto id = ctx->id->getText();
    const double x = std::stod(ctx->x->getText());
    const double y = std::stod(ctx->y->getText());
    const double z = std::stod(ctx->y->getText());

    std::cout << "found:\n"
    << "cmd: " << cmd << "\n"
    << "id: " << id << "\n" << std::fixed
    << "x: " << x << "\n"
    << "y: " << y << "\n"
    << "z: " << z << "\n";
  }

  auto exitComment(fbdParser::CommentContext* ctx) -> void override {
    // do something with the comment context
    //std::cout << ctx->getText() << std::endl;
  }
};

ParserFBD::ParserFBD(std::string filename) : filename_(std::move(filename)) {}

void ParserFBD::Parse() {
  std::ifstream input_file(filename_, std::ios::in);
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
}

}  // namespace holodeckx
