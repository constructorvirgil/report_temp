cmake_minimum_required(VERSION 3.15)
project(server C)

set(CMAKE_C_STANDARD 99)

add_executable(server main.c tempdb.c packer.c tdata.c epoll_server.c epoll_server.h)
target_link_libraries(server sqlite3 pthread)