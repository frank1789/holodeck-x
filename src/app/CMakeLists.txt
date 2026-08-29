set(EXECUTABLE_TARGET_NAME "HolodeckX")

add_executable(${EXECUTABLE_TARGET_NAME})
target_sources(${EXECUTABLE_TARGET_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/src/ui_renderer.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui_layout.cc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui_menu.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dockspace_host.cc
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application.hh
 # ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_renderer.hh
 # ${CMAKE_CURRENT_SOURCE_DIR}/include/renderer.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_layout.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_menu.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/abstract_panel.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/dockspace_host.hh
   ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panel_manager.hh
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
  PRIVATE
    fmt::fmt
    SDL3::SDL3
    imgui::imgui
    Backward::Backward
    Eigen3::Eigen
    #Vulkan::Vulkan
)

target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE -g3)

install(TARGETS ${EXECUTABLE_TARGET_NAME} DESTINATION bin)
