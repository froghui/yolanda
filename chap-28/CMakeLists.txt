add_executable(poll-server-multithreads poll-server-multithreads.c)
target_link_libraries(poll-server-multithreads yolanda)

add_executable(test01 test01.c)
target_link_libraries(test01 yolanda)