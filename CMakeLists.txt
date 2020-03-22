cmake_minimum_required(VERSION 3.1)
project(yolanda C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

# check epoll and add config.h for the macro compilation
include(CheckSymbolExists)
check_symbol_exists(epoll_create "sys/epoll.h" EPOLL_EXISTS)
if (EPOLL_EXISTS)
    # Linux下设置为epoll
    set(EPOLL_ENABLE 1 CACHE INTERNAL "enable epoll")

    # Linux下也设置为poll
#    set(EPOLL_ENABLE "" CACHE INTERNAL "not enable epoll")
else ()
    set(EPOLL_ENABLE "" CACHE INTERNAL "not enable epoll")
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(lib/)
add_subdirectory(chap-4/)
add_subdirectory(chap-5/)
add_subdirectory(chap-6/)
add_subdirectory(chap-7/)
add_subdirectory(chap-11)
add_subdirectory(chap-12)
add_subdirectory(chap-13)
add_subdirectory(chap-14/)
add_subdirectory(chap-15)
add_subdirectory(chap-16)
add_subdirectory(chap-17)
add_subdirectory(chap-18)
add_subdirectory(mid-homework)
add_subdirectory(chap-20)
add_subdirectory(chap-21)
add_subdirectory(chap-22)
add_subdirectory(chap-23)
add_subdirectory(chap-25)
add_subdirectory(chap-26)
add_subdirectory(chap-27)
add_subdirectory(chap-28)
add_subdirectory(chap-29)
add_subdirectory(chap-30)
add_subdirectory(chap-34)

