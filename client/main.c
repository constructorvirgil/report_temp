#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "tdata.h"
#include "packer.h"

#define MACID "PI0001"
#define TPATH "../temp.txt"

int main(int argc, char **argv)
{
    if(argc < 1){
        printf("too few arguments!\n");
        return -1;
    }

    int                     conn_fd = -1;
    struct sockaddr_in      serv_addr;
    char                    buf[1024];
    if(argv[1] == NULL){
        printf("invalid argument!\n");
        return -1;
    }
    int port = atoi(argv[1]);
    if(port < 0){
        printf("arguments error!\n");
        return -1;
    }

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
    {
        printf("create socket failure: %s\n", strerror(errno));
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton( "127.0.0.1", &serv_addr.sin_addr );

    printf("connect to port %d\n",port);

    if( connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))  < 0)
    {
        perror("Cannot connect server!\n");
        close(conn_fd);
        exit(EXIT_FAILURE);
    }

    printf("connected successfully!\n");

    while(1)
    {
        int r;
        struct mac_context m;
        struct tdata td;
        struct tbyte tb;
        strcpy(m.id,MACID);
        strcpy(m.tpath,TPATH);

        if((r = get_data(&m,&td)) < 0){
            printf("get temp pdata failed!\n");
            close(conn_fd);
            return -1;
        }
        tdata2tbyte(&td,&tb);

        struct pack_data pk;
        memcpy(pk.data,tb.byte,sizeof(tb.byte));
        pk.len = sizeof(tb.byte);
        send_pack(conn_fd,&pk);
        /*if((r = send_pack(&pk)) < 0){
            close(conn_fd);
            return -2;
        }*/

        sleep(1);
    }
    close(conn_fd);
}