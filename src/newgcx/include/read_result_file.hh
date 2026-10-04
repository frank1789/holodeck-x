#ifndef HOLODECK_X_READ_RESULT_FILE_HH_
#define HOLODECK_X_READ_RESULT_FILE_HH_

#include <expected>
#include <string>

#include "element_type.hh"
#include "parser.hh"

namespace holodeck_x {

class ParseResultFile : public Parser<ParseResultFile> {
 public:
  explicit ParseResultFile(std::string filename) noexcept;

  std::expected<void, ParseError> Parse();

 private:
  std::string filename_;
};

}  // namespace holodeck_x

#endif  // HOLODECK_X_READ_RESULT_FILE_HH_
