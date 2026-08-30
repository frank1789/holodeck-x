set(LIBRARY_TARGET_NAME "ImViewGuizmo")

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_sources(${LIBRARY_TARGET_NAME}
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src/imviewguizmo_impl.cc)


target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    "${imviewguizmo_SOURCE_DIR}"
)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE     imgui::imgui glm::glm)
target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC IMVIEWGUIZMO_IMPLEMENTATION)
