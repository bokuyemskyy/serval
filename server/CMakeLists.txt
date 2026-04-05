include(FetchContent)

# fetch CLI options library
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG        v3.3.1
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(cxxopts)

# fetch YAML parser library
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

file(GLOB SRCS
        src/*.cpp
)

add_executable(serval ${SRCS})

target_include_directories(serval PRIVATE include)

target_link_libraries(serval PUBLIC libserval cxxopts::cxxopts yaml-cpp::yaml-cpp)
