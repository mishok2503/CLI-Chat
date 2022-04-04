set(SOURCES
    src/test.cpp
    src/main.cpp
)

add_executable(server ${SOURCES})

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
