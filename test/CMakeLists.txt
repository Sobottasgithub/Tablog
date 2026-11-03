cmake_minimum_required(VERSION 3.10)
project(tablog-test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "./*.cpp" "./*.h")

add_executable(tablog-test ${SOURCES})

target_link_libraries(tablog-test PRIVATE tablog)

install(TARGETS tablog-test DESTINATION bin)
