//
// Created by virgil on 3/6/20.
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
#include <pthread.h>
#include <sys/mman.h>
#include <sys/select.h>
#include "tempdb.h"
#include "sel_server.h"
#include "packer.h"

int listen_fd = -1;
struct tdata* datas;
pthread_mutex_t* m;

int server_init(int port)
{
    struct sockaddr_in      serv_addr;
    int fds_array[1024];
    fd_set rdset;


    //init socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0 )
    {
        perror("Failed to create socket!");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //set socket addr reusable
    int ok = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1)
    {
        perror("Set sock failed!");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if(bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        perror("Bind socket failed!");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    listen(listen_fd, 5);
    printf("server[%d] is listening on port %d\n", listen_fd,port);

    m = (pthread_mutex_t* )mmap(NULL,sizeof(pthread_mutex_t),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
    //m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    //make mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(m,&attr);
    pthread_mutexattr_destroy(&attr);
    //pthread_mutex_lock(&tasks[i]->m);

    memset(fds_array,0,ARRAY_SIZE(fds_array));

    for(int i=0; i<ARRAY_SIZE(fds_array) ; i++)
    {
        fds_array[i]=-1;
    }
    fds_array[0] = listen_fd;

    while(1)
    {

        int i,rv,new_fd;
        int maxfd = 0;

        FD_ZERO(&rdset);
        for(i=0; i<ARRAY_SIZE(fds_array) ; i++)
        {
            if( fds_array[i] < 0 )
                continue;
            maxfd = fds_array[i]>maxfd ? fds_array[i] : maxfd;
            FD_SET(fds_array[i], &rdset);
        }

        /* program will blocked here */
        rv = select(maxfd+1, &rdset, NULL, NULL, NULL);
        if(rv < 0)
        {
            perror("Select failed!");
            break;
        }
        else if(rv == 0)
        {
            printf("select get timeout\n");
            continue;
        }

        /* listen socket get event means new client start connect now */
        int found;
        if ( FD_ISSET(listen_fd, &rdset) )
        {
            if( (new_fd=accept(listen_fd, (struct sockaddr *)NULL, NULL)) < 0)
            {
                printf("accept new client failure: %s\n", strerror(errno));
                continue;
            }
            found = 0;
            for(i=0; i<ARRAY_SIZE(fds_array) ; i++)
            {
                if( fds_array[i] < 0 )
                {
                    printf("accept new client[%d] and add it into array\n", new_fd );
                    fds_array[i] = new_fd;
                    found = 1;
                    break;
                }
            }
            if( !found ){
                printf("accept new client[%d] but full, so refuse it\n", new_fd);
                close(new_fd);
            }
        }
        else /* data arrive from already connected client */
        {
            for(i=0; i<ARRAY_SIZE(fds_array); i++)
            {
                if( fds_array[i]<0 || !FD_ISSET(fds_array[i], &rdset) )
                    continue;

                if(recv_handle(fds_array[i])<0){
                    printf("socket[%d] read failure or get disconncet.\n", fds_array[i]);
                    close(fds_array[i]);
                    fds_array[i] = -1;
                }

            }
        }

    }

    close(listen_fd);
}


int recv_handle(int fd)
{
    int r;
    struct pack_data pk;
    struct tdata td;
    if((r = recv_pack(fd,&pk)) < 0){
        close(fd);
        return -1;
    }

    struct tbyte tb;
    memcpy(tb.byte,pk.data,pk.len);
    tbyte2tdata(&tb,&td);

    pthread_mutex_lock(m);
    if(tpdb_report(&td) == 0){
        printf("insert: [%s][%s][%s]\n",td.mac,td.dtime,td.temp);
    }
    pthread_mutex_unlock(m);

    return 0;
}


