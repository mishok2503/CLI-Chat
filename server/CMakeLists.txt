set(SOURCES
    src/main.cpp
    src/server.cpp
    src/connection_data.cpp
)

add_executable(server ${SOURCES})

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
