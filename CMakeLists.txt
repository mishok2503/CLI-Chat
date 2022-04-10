cmake_minimum_required(VERSION 3.17)

project(CLI-Chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(server)
add_subdirectory(client)
