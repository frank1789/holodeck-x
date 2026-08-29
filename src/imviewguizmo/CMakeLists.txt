set(LIBRARY_TARGET_NAME "ImViewGuizmo")

add_library(${LIBRARY_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_include_directories(${LIBRARY_TARGET_NAME}
    INTERFACE
    "${imviewguizmo_SOURCE_DIR}"
)
