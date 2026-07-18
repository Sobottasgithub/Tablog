cmake_minimum_required(VERSION 3.10)
project(tablog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DEF_TABLOG "configure library" ON)
option(DEF_CLIENT "configure client" ON)

if(DEF_TABLOG)
    add_subdirectory(lib/tablog)
endif()

if(DEF_CLIENT)
    add_subdirectory(test/client)
endif()
