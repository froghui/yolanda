add_executable(udpclient udpclient.c)
target_link_libraries(udpclient yolanda)

add_executable(udpserver udpserver.c)
target_link_libraries(udpserver yolanda)

add_executable(udpclient1 udpclient1.c)
target_link_libraries(udpclient1 yolanda)

add_executable(udpserver3 udpserver3.c)
target_link_libraries(udpserver3 yolanda)

add_executable(udpclient3 udpclient3.c)
target_link_libraries(udpclient3 yolanda)


