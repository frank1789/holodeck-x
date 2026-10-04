
#include "parser/mapped_file.hh"

#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#endif

namespace holodeck::parser {

MmappedFile::~MmappedFile() noexcept {
  if (addr && addr != MAP_FAILED) {
    munmap(addr, length);
  }
}

std::expected<int, ErrorStatus> OpenFile(const char* filepath) noexcept {
  int fd = open(filepath, O_RDONLY);
  if (fd == -1) {
    return std::unexpected(ErrorStatus::kOpenFailed);
  }
  return fd;
}

std::expected<MmappedFile, ErrorStatus> MapDescriptor(int fd) noexcept {
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    close(fd);
    return std::unexpected(ErrorStatus::kStatFailed);
  }

  void* mapped_addr = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  if (mapped_addr == MAP_FAILED) {
    return std::unexpected(ErrorStatus::kMmapFailed);
  }

  return MmappedFile{mapped_addr, static_cast<std::size_t>(sb.st_size)};
}

}  // namespace holodeck::parser
