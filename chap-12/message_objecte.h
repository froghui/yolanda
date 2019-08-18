//
// Created by shengym on 2019-08-02.
//

#ifndef YOLANDA_MESSAGE_OBJECTE_H
#define YOLANDA_MESSAGE_OBJECTE_H


typedef struct {
    u_int32_t type;
    char data[1024];
} messageObject;

#define MSG_PING          1
#define MSG_PONG          2
#define MSG_TYPE1        11
#define MSG_TYPE2        21


#endif //YOLANDA_MESSAGE_OBJECTE_H
