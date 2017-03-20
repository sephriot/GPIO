cmake_minimum_required (VERSION 3.0.2)
project (gpio)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Base project
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

set (INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
foreach (headerFile ${HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

include_directories(${INCLUDE_DIRS})

add_library (gpio ${SOURCES})
target_include_directories (gpio PUBLIC ${INCLUDE_DIRS})
