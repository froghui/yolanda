add_executable(tcpclient tcpclient.c)
target_link_libraries(tcpclient yolanda)

add_executable(tcpserver tcpserver.c)
target_link_libraries(tcpserver yolanda)

