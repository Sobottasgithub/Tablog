cmake_minimum_required(VERSION 3.10)
project(tablog-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "./*.cpp" "./*.h")

add_executable(tablog-client ${SOURCES})

target_link_libraries(tablog-client PRIVATE tablog)

install(TARGETS tablog-client DESTINATION bin)
