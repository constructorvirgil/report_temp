cmake_minimum_required(VERSION 3.15)
project(server C)

set(CMAKE_C_STANDARD 99)

add_executable(server main.c tempdb.c multipro_server.c packer.c tdata.c)
target_link_libraries(server sqlite3)