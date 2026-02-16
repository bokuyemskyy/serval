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
        src/*.cpp
)

add_library(libserval ${SRCS})

target_include_directories(libserval PUBLIC include)

target_link_libraries(libserval PUBLIC socketpoll cxxopts::cxxopts tomlplusplus::tomlplusplus)