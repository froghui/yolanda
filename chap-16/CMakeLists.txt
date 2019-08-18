add_executable(streamserver streamserver.c)
target_link_libraries(streamserver yolanda)

add_executable(streamclient streamclient.c)
target_link_libraries(streamclient yolanda)
