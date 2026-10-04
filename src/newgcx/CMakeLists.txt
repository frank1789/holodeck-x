set(LIBRARY_TARGET_NAME "newcgx")

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_sources(${LIBRARY_TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/mapped_file.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/read_result_file.cc
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/element_type.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input_mode.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/parser.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mapped_file.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/read_result_file.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utilities.hh
)

add_executable(cgx src/main.cc)
target_link_libraries(cgx PRIVATE ${LIBRARY_TARGET_NAME})
