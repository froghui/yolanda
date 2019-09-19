add_executable(telnet-client telnet-client.c)
target_link_libraries(telnet-client yolanda)

add_executable(telnet-server telnet-server.c)
target_link_libraries(telnet-server yolanda)
