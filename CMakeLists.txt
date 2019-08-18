cmake_minimum_required(VERSION 3.1)
project(yolanda C CXX)

set(CMAKE_CXX_STANDARD 11)

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib/)
add_subdirectory(chap-4/)
add_subdirectory(chap-5/)
add_subdirectory(chap-6/)
add_subdirectory(chap-7/)
add_subdirectory(chap-11)
add_subdirectory(chap-12)
add_subdirectory(chap-13)
add_subdirectory(chap-15)
add_subdirectory(chap-16)
add_subdirectory(chap-17)
add_subdirectory(chap-18)

