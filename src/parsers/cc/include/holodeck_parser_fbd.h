#ifndef HOLODECK_X_CC_PARSERS_PARSER_FBD_H_
#define HOLODECK_X_CC_PARSERS_PARSER_FBD_H_

#include "holodeckx_parser.h"

namespace holodeckx {

class ParserFBD : public Parser<ParserFBD> {
 public:
  explicit ParserFBD(std::string filename);

  void Parse();

 private:
  std::string filename_;
};

}  // namespace holodeckx

#endif  //  HOLODECK_X_CC_PARSERS_PARSER_FBD_H_
