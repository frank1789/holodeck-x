#ifndef HOLODECK_X_NEW_CGX_PARSER_HH_
#define HOLODECK_X_NEW_CGX_PARSER_HH_

#include <cstdint>

namespace holodeck::parser {

enum class ParseError : std::uint8_t {
  kInvalidInput,
  kFileNotFound,
  kParseError,
};

template <typename Derived>
class Parser {
 public:
 protected:
  constexpr auto Self() noexcept -> Derived& {
    return static_cast<Derived&>(*this);
  }

  constexpr auto Self() const noexcept -> const Derived& {
    return static_cast<const Derived&>(*this);
  }

 private:
  friend Derived;
  constexpr Parser() noexcept = default;
};

}  // namespace holodeck::parser

#endif  // HOLODECK_X_NEW_CGX_PARSER_HH_
