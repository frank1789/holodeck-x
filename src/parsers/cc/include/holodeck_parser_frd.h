#ifndef HOLODECK_X_CC_PARSERS_PARSER_FRD_H_
#define HOLODECK_X_CC_PARSERS_PARSER_FRD_H_

#include "holodeckx_parser.h"

namespace holodeckx {

class ParserResult : public Parser<ParserResult> {
 public:
  ParserResult() = default;
  explicit ParserResult(std::string filename);

  auto Parse() -> void;

 private:
  std::string filename_;
};

}  // namespace holodeckx

#endif  //  HOLODECK_X_CC_PARSERS_PARSER_FRD_H_
