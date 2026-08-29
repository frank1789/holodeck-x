set(EXECUTABLE_TARGET_NAME "HolodeckX")

add_executable(${EXECUTABLE_TARGET_NAME})
target_sources(${EXECUTABLE_TARGET_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/application.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui_layout.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui_menu.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dockspace_host.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/analysis_tree.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/group_and_selection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/main_panel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/mesh_warning.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/output_log.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/properties_editor.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/panels/solver_control.cc
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/application.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_layout.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui_menu.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/abstract_panel.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/dockspace_host.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panel_manager.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/analysis_tree.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/group_and_selection.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/main_panel.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/mesh_warning.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/output_log.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/properties_editor.hh
${CMAKE_CURRENT_SOURCE_DIR}/include/ui/panels/solver_control.hh
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
  PRIVATE
    fmt::fmt
    SDL3::SDL3
    imgui::imgui
    Backward::Backward
    Eigen3::Eigen
    glm::glm-header-only
ImViewGuizmo
    #Vulkan::Vulkan
)

add_dependencies(${EXECUTABLE_TARGET_NAME} ImViewGuizmo)


target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE -g3)

install(TARGETS ${EXECUTABLE_TARGET_NAME} DESTINATION bin)
