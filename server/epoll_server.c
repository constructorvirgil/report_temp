//
// Created by virgil on 3/7/20.
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
#include <sys/epoll.h>
#include "tempdb.h"
#include "packer.h"
#include "epoll_server.h"


int listen_fd = -1;
struct tdata* datas;
pthread_mutex_t* m;

int server_init(int port)
{
    struct sockaddr_in      serv_addr;
    int epollfd;
    struct epoll_event event;
    struct epoll_event event_array[MAX_EVENTS] = {0};
    int events;

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

    if( (epollfd=epoll_create(MAX_EVENTS)) < 0 )
    {
        perror("epoll_create() failed!");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    //event.events = EPOLLIN|EPOLLET;
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    if( epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd, &event) < 0)
    {
        perror("Epoll add listen socket failed!");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        int rv;
        events = epoll_wait(epollfd, event_array, MAX_EVENTS, -1);
        if(events < 0)
        {
            perror("Epoll failed!");
            break;
        }
        else if(events == 0)
        {
            perror("Epoll timeout!");
            continue;
        }

        for(int i=0; i<events; i++)
        {
            if ( (event_array[i].events&EPOLLERR) || (event_array[i].events&EPOLLHUP) )
            {
                printf("epoll_wait get error on fd[%d]: %s\n", event_array[i].data.fd, strerror(errno));
                epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                close(event_array[i].data.fd);
                continue;
            }


            if( event_array[i].data.fd == listen_fd )
            {
                int new_fd;
                if( (new_fd=accept(listen_fd, (struct sockaddr *)NULL, NULL)) < 0)
                {
                    printf("accept new client failure: %s\n", strerror(errno));
                    continue;
                }
                event.data.fd = new_fd;
                event.events = EPOLLIN;
                if( epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &event) < 0 )
                {
                    perror("Epoll add client socket failed!");
                    close(event_array[i].data.fd);
                    continue;
                }
                printf("epoll add new client socket[%d] ok.\n", new_fd);
            }
            else /* already connected client socket get data incoming */
            {
                //recv_handle...
                if(recv_handle(event_array[i].data.fd) < 0){
                    printf("Client[%d] disconnected!\n",event_array[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                    close(event_array[i].data.fd);
                    continue;
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


