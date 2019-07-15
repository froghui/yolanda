set(yolanda_lib
        log.c
        sock_ntop.c
        )

add_library(yolanda ${yolanda_lib})
target_link_libraries(yolanda pthread)

install(TARGETS yolanda DESTINATION lib)

#file(GLOB HEADERS "*.h")
#install(FILES ${HEADERS} DESTINATION include/)

