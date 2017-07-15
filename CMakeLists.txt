cmake_minimum_required(VERSION 3.7)
project(cur)

set(CMAKE_CXX_STANDARD 11)



set(SOURCE_FILES main.cpp HTTPDownloader.cpp HTTPDownloader.h)
add_executable(cur ${SOURCE_FILES})
target_link_libraries(cur curl)