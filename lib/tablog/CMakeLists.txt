cmake_minimum_required(VERSION 3.10)
project(tablog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
# file(GLOB_RECURSE HEADERS "include/*.h")

add_library(tablog SHARED ${SOURCES} ${LIBTASN1_LIBRARIES})

target_include_directories(tablog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS tablog DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
