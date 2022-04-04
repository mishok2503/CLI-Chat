cmake_minimum_required(VERSION 3.17)

project(CLI-Chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Werror)

add_subdirectory(server)
add_subdirectory(client)
