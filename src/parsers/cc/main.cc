#include <iostream>

#include "holodeck_parser_fbd.h"
#include "holodeck_parser_frd.h"

auto main(int argc, char** argv) -> int {
  std::string filename =
      "/Users/francesco/Downloads/cgx_2.23.all/CalculiX/cgx_2.23/examples/cad/"
      "halter.fbd";  // Replace with your actual filename
  holodeckx::ParserFBD parser(filename);
  holodeckx::ParserResult parser_result(
      "/Users/francesco/Downloads/cgx_2.23.all/CalculiX/cgx_2.23/examples/"
      "result.frd");

  try {
    parser.Parse();
    parser_result.Parse();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
