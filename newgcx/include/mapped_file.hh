#ifndef HOLODECK_X_MAPPED_FILE_HH_
#define HOLODECK_X_MAPPED_FILE_HH_

#include <cstddef>
#include <cstdint>
#include <expected>

namespace holodeck::parser {

enum class ErrorStatus : std::uint8_t { kOpenFailes, kStatFailed, kMmapFailed };

struct MmappedFile {
  void *addr{nullptr};
  std::size_t length{};

  ~MmappedFile() noexcept;

  MmappedFile(const MmappedFile &) = delete;
  MmappedFile &operator=(const MmappedFile &) = delete;

  MmappedFile(MmappedFile &&other) noexcept
      : addr(other.addr), length(other.length) {
    other.addr = nullptr;
  }
};

std::expected<int, ErrorStatus> OpenFile(const char *filepath);

std::expected<MmappedFile, ErrorStatus> MapDescriptor(int fd);

} // namespace holodeck::parser

#endif // HOLODECK_X_MAPPED_FILE_HH_
