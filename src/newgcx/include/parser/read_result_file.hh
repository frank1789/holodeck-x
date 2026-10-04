#ifndef HOLODECK_X_READ_RESULT_FILE_HH_
#define HOLODECK_X_READ_RESULT_FILE_HH_

#include <expected>
#include <string>

#include "element_type.hh"
#include "parser.hh"

namespace holodeck::parser {

class ResultFileParser : public Parser<ResultFileParser> {
public:
  explicit ResultFileParser(std::string filename) noexcept;

  std::expected<void, ParseError> Parse();

private:
  std::string filename_;
};

} // namespace holodeck::parser

#endif // HOLODECK_X_READ_RESULT_FILE_HH_
