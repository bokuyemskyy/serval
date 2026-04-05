include(FetchContent)

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

target_link_libraries(libserval PUBLIC socketpoll)