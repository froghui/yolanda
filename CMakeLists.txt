cmake_minimum_required(VERSION 3.14)
project(yolanda)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(udp)

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


add_executable(udpclient udp/udpclient.c lib/log.c lib/common.h lib/sock_ntop.c)

add_executable(udpserver udp/udpserver.c lib/log.c lib/common.h)
