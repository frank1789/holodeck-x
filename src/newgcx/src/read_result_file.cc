#include "read_result_file.hh"

#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#endif

namespace holodeck_x {

ParseResultFile::ParseResultFile(std::string filename) noexcept
    : filename_(std::move(filename)) {}

std::expected<void, ParseError> ParseResultFile::Parse() {
  // Open the file for reading
  int fd = open(filename_.c_str(), O_RDONLY);
  if (fd == -1) {
    return std::unexpected(ParseError::kInvalidInput);
  }

  // Determine the size of the file
  struct stat file_info;
  if (fstat(fd, &file_info) == -1) {
    close(fd);
    perror("fstat failed.");
    return std::unexpected(ParseError::kInvalidInput);
  }
  off_t file_size = file_info.st_size;

  // Map the file into memory for reading
  char* mapped_data =
      (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mapped_data == MAP_FAILED) {
    close(fd);
    perror("mmap");
    return std::unexpected(ParseError::kInvalidInput);
  }

  // Close the file (not needed after mapping)
  close(fd);

  // Now you can access the contents of the file using mapped_data
  // For example, printing the file contents:
  printf("File Contents:\n%s\n", mapped_data);

  // Unmap the memory
  if (munmap(mapped_data, file_size) == -1) {
    perror("munmap");
  }

  return std::expected<void, ParseError>({});
}

}  // namespace holodeck_x
