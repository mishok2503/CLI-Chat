set(SOURCES
    src/test.cpp
    src/main.cpp
)

add_executable(client ${SOURCES})

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
