set(SOURCES
    src/main.cpp
    src/client.cpp
    src/connection_data.cpp
)

add_executable(client ${SOURCES})

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client PRIVATE Threads::Threads)
