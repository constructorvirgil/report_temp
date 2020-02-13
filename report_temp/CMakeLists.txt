cmake_minimum_required(VERSION 3.15)
project(report_temp C)

set(CMAKE_C_STANDARD 99)

add_executable(report_temp main.c tempdb.c tempdb.h simple_server.c simple_server.h)
target_link_libraries(report_temp sqlite3)