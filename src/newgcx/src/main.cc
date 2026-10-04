#include <cstdlib>
#include <print>
#include <format>

#include "parser/read_result_file.hh"

int main(int argc, char **argv)
{
    holodeck::parser::ResultFileParser parser("/Users/francesco/Downloads/cgx_2.23.all 2/CalculiX/cgx_2.23/examples/result.frd");
    auto result = parser.Parse();
    if (!result) {
        std::printf("Error parsing file: %d\n", static_cast<int>(result.error()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
