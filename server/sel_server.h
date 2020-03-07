//
// Created by virgil on 3/6/20.
//

#ifndef SERVER_SEL_SERVER_H
#define SERVER_SEL_SERVER_H


#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
int recv_handle(int fd);
int server_init(int port);


#endif //SERVER_SEL_SERVER_H

