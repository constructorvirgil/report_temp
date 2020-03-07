//
// Created by virgil on 3/7/20.
//

#ifndef SERVER_EPOLL_SERVER_H
#define SERVER_EPOLL_SERVER_H

#define MAX_EVENTS 512
int recv_handle(int fd);
int server_init(int port);


#endif //SERVER_EPOLL_SERVER_H
