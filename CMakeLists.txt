cmake_minimum_required(VERSION 3.20)

project(serval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# fetch CLI options library
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.3.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cxxopts)

# fetch TOML parser library
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# fetch socket + polling library I implemented earlier
FetchContent_Declare(
  socketpoll
  GIT_REPOSITORY https://github.com/bokuyemskyy/socketpoll.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(socketpoll)

file(GLOB SRCS
        server/src/*.cpp
)

add_executable(serval ${SRCS}
        lib/src/http_protocol.cpp
        lib/include/http_protocol.hpp)

target_include_directories(serval PRIVATE server/include)

target_link_libraries(serval PUBLIC socketpoll cxxopts::cxxopts tomlplusplus::tomlplusplus)

# Docs
option(BUILD_DOCS "Build documentation with Doxygen" OFF)

if(BUILD_DOCS)
    find_package(Doxygen QUIET)

    if(NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found. Skipping docs.")
        return()
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
endif()

# Tests
option(BUILD_TESTING "Build tests" OFF)
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()