set(EXECUTABLE_TARGET_NAME "app")

add_executable(${EXECUTABLE_TARGET_NAME})
target_sources(${EXECUTABLE_TARGET_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/src/ui_renderer.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cc
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application.hh
 # ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_renderer.hh
 # ${CMAKE_CURRENT_SOURCE_DIR}/include/renderer.hh
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
  PRIVATE
    fmt::fmt
    SDL3::SDL3
    imgui::imgui
    Backward::Backward
    #Vulkan::Vulkan
)

target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE -g3)

install(TARGETS ${EXECUTABLE_TARGET_NAME} DESTINATION bin)
