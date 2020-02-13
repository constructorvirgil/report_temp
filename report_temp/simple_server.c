//
// Created by virgil on 2/13/20.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tempdb.h"
#include "simple_server.h"


int listen_fd = -1;

int server_init(int port)
{
    struct sockaddr_in      serv_addr;
    char                    buf[1024];

    //init socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0 )
    {
        perror("Failed to create socket!\n");
        exit(SERVER_ERROR);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ok = 1;
    if( setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1)
    {
        perror("Set sock failed!\n");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if( bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("Bind socket failed!\n");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    listen(listen_fd, 5);
    printf("server[%d] is listening on port %d\n", listen_fd,port);

    while(1)
    {
        int new_fd = accept(listen_fd, NULL, NULL);
        if(new_fd < 0)
        {
            perror("Accept client failed!");
            exit(SERVER_ERROR);
        }

        //create a progress to heandle fd
        int ret = fork();
        if(ret < 0)
        {
            perror("Accept client failed!");
            exit(SERVER_ERROR);
        }
        if(ret == 0)
        {
            continue;
        }

        //child progress
        recv_handle(new_fd);
    }

    close(listen_fd);
}


int recv_temp(int fd,char* mac,char* datetime,char* temp)
{

    char buf[LEN_BUF] = {0};
    int size = 1+LEN_MAC+LEN_DATETIME+LEN_TEMP;

    int n = read(fd,buf,size);
    if(n < 0){
        return -1;
    }
    while(n < size)
    {
        int new_n = read(fd,temp+n,size-n);
        if(new_n < 0){
            return -1;
        }
        n += new_n;
    }

    //unpack

    for(int i=0;i<LEN_MAC;i++)
    {
        mac[i] = buf[i];
    }
    for(int i = 0;i<LEN_DATETIME;i++)
    {
        datetime[i] = buf[LEN_MAC + i];
    }
    for(int i = 0;i<LEN_TEMP;i++)
    {
        temp[i] = buf[LEN_MAC+LEN_DATETIME+i];
    }

    return 0;
}

int recv_handle(int new_fd)
{
    while(1)
    {
        int rc = 0;
        char mac[LEN_MAC+1] = {0};
        char datetime[LEN_DATETIME+1] = {0};
        char temp[LEN_TEMP+1] = {0};

        char flag = 0;
        do{
            rc = read(new_fd,&flag,1);
            if(rc < 0){
                perror("Client disconnected!\n");
                close(new_fd);
                exit(EXIT_FAILURE);
            }
        }while(flag != (char)0xde);

        rc = recv_temp(new_fd,mac,datetime,temp);
        if(rc != 0){
            perror("Client disconnected!\n");
            close(new_fd);
            exit(EXIT_FAILURE);
        }

        tpdb_report(mac,datetime,temp);

    }
}
