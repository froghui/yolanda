add_executable(tcpclient tcpclient.c)
target_link_libraries(tcpclient yolanda)

add_executable(tcpserver tcp_server.c)
target_link_libraries(tcpserver yolanda)

