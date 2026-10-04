#include "parser/read_result_file.hh"

#include <iostream>
#include <ranges>

#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#endif

#include "parser/mapped_file.hh"

namespace holodeck::parser {

ResultFileParser::ResultFileParser(std::string filename) noexcept
    : filename_(std::move(filename)) {}

std::expected<void, ParseError> ResultFileParser::Parse() {
  auto file_result =
      OpenFile(filename_.c_str())
          .and_then(MapDescriptor)
          .or_else(
              [](ErrorStatus error) -> std::expected<MmappedFile, ErrorStatus> {
                switch (error) {
                  case ErrorStatus::kOpenFailed:
                    std::cerr << "Failed to open file: " << strerror(errno)
                              << std::endl;
                    break;
                  case ErrorStatus::kStatFailed:
                    std::cerr
                        << "Failed to get file status: " << strerror(errno)
                        << std::endl;
                    break;
                  case ErrorStatus::kMmapFailed:
                    std::cerr
                        << "Failed to memory-map file: " << strerror(errno)
                        << std::endl;
                    break;
                }

                return std::unexpected(error);
              });

  const auto& content = file_result.value();
  std::string_view file_contents(static_cast<const char*>(content.addr),
                                 content.length);

  auto lines = file_contents | std::views::split('\n') | std::views::enumerate;
  for (auto [line_number, line] : lines) {
    std::string_view line_view(line.begin(), line.end());
    if (line_view.empty() && line.back() == '\r') {
      line_view.remove_suffix(1);
    }
    std::cout << "Line " << line_number << ": " << line_view << "\n";
    // Process each line as needed
  }
  // // Open the file for reading
  // int fd = open(filename_.c_str(), O_RDONLY);
  // if (fd == -1) {
  //   return std::unexpected(ParseError::kInvalidInput);
  // }

  // // Determine the size of the file
  // struct stat file_info;
  // if (fstat(fd, &file_info) == -1) {
  //   close(fd);
  //   perror("fstat failed.");
  //   return std::unexpected(ParseError::kInvalidInput);
  // }
  // off_t file_size = file_info.st_size;

  // // Map the file into memory for reading
  // char* mapped_data =
  //     (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  // if (mapped_data == MAP_FAILED) {
  //   close(fd);
  //   perror("mmap");
  //   return std::unexpected(ParseError::kInvalidInput);
  // }

  // // Close the file (not needed after mapping)
  // close(fd);

  // // Now you can access the contents of the file using mapped_data
  // // For example, printing the file contents:
  // printf("File Contents:\n%s\n", mapped_data);

  // // Unmap the memory
  // if (munmap(mapped_data, file_size) == -1) {
  //   perror("munmap");
  // }

  return std::expected<void, ParseError>({});
}

}  // namespace holodeck::parser
