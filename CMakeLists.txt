cmake_minimum_required(VERSION 3.10)
project(tablog)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DEF_TABLOG "configure library" ON)
option(DEF_TEST "configure test" ON)

if(DEF_TABLOG)
    add_subdirectory(lib/tablog)
endif()

if(DEF_TEST)
    add_subdirectory(test)
endif()
