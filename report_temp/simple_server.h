//
// Created by virgil on 2/13/20.
//

#ifndef REPORT_TEMP_SIMPLE_SERVER_H
#define REPORT_TEMP_SIMPLE_SERVER_H

#define SERVER_ERROR 1
#define SERVER_ERROR_LEN 2
#define CLIENT_ERROR 3
#define CLIENT_DISCONNECTED 4
#define LEN_BUF 64


int recv_handle(int new_fd);
int recv_temp(int fd,char* mac,char* datetime,char* temp);
int server_init(int port);


#endif //REPORT_TEMP_SIMPLE_SERVER_H
