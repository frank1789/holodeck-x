cmake_minimum_required(VERSION 3.27..4.3 FATAL_ERROR)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif (POLICY CMP0141)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
project(HolodeckX
  LANGUAGES C CXX
  VERSION 0.1.0
  DESCRIPTION "A simple program with gui"
)

include(GNUInstallDirs)
include(CheckIPOSupported)
include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(BUILD_SHARED_LIBS OFF "")
option(BUILD_TESTING OFF "")
option(BUILD_DOCUMENTATION OFF)

if(BUILD_TESTING)
  enable_testing()
  include(CTest)
  find_package(Gtest CONFIG REQUIRED)
endif()

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
  endif()
endif()

FetchContent_Declare(
  imviewguizmo
  GIT_REPOSITORY https://github.com/Ka1serM/ImViewGuizmo
  GIT_TAG v1.0.2
)
FetchContent_MakeAvailable(imviewguizmo)


find_package(Vulkan REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Backward CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_subdirectory(src)
