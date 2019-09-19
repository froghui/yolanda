if (EPOLL_EXISTS)
    add_executable(epoll01 epoll01.c)
    target_link_libraries(epoll01 yolanda)

    add_executable(epoll02 epoll02.c)
    target_link_libraries(epoll02 yolanda)

    add_executable(epoll03 epoll03.c)
    target_link_libraries(epoll03 yolanda)
endif ()