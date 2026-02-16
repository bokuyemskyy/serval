file(GLOB SRCS
        src/*.cpp
)

add_executable(serval ${SRCS})

target_include_directories(serval PRIVATE include)

target_link_libraries(serval PUBLIC libserval)
