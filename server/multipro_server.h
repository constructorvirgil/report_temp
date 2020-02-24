//
// Created by virgil on 2/13/20.
//

#ifndef REPORT_TEMP_MULTIPRO_SERVER_H
#define REPORT_TEMP_MULTIPRO_SERVER_H

int recv_handle(int new_fd);
int recv_temp(int fd,char* mac,char* datetime,char* temp);
int server_init(int port);


#endif //REPORT_TEMP_MULTIPRO_SERVER_H
