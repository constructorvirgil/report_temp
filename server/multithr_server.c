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
#include "tempdb.h"
#include "packer.h"
#include "multithr_server.h"


int listen_fd = -1;
struct tdata* datas;
pthread_mutex_t* m;


int server_init(int port)
{
    struct sockaddr_in      serv_addr;

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

    while(1)
    {
        struct sockaddr_in new_addr;
        int len;
        int new_fd;
        pthread_t tid;
        if((new_fd = accept(listen_fd, (struct sockaddr *)&new_addr, &len)) < 0){
            perror("Accept client failed!");
            close(listen_fd);
            exit(EXIT_FAILURE);
        }

        printf("accpet new client[%d] successfully\n", new_fd);

        //thread_start(&tid, thread_worker, (void *)clifd);
        //thread_start

        pthread_attr_t thread_attr;
        if( pthread_attr_init(&thread_attr) )
        {
            perror("pthread_attr_init() failure: %s");
            close(new_fd);
            pthread_attr_destroy(&thread_attr);
            continue;
        }
        if( pthread_attr_setstacksize(&thread_attr, 120*1024) )
        {
            perror("pthread_attr_setstacksize() failure: %s");
            close(new_fd);
            pthread_attr_destroy(&thread_attr);
            continue;

        }
        if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )
        {
            perror("pthread_attr_setdetachstate() failure: %s");
            close(new_fd);
            pthread_attr_destroy(&thread_attr);
            continue;

        }


        if( pthread_create(&tid, &thread_attr, recv_handle, (void *)new_fd) )
        {
            perror("Create thread failure: %s");
            close(new_fd);
            pthread_attr_destroy(&thread_attr);
            continue;
        }

        pthread_attr_destroy(&thread_attr);


    }

    close(listen_fd);
}


void* recv_handle(void* ctx)
{
    int fd = (int)ctx;
    while(1)
    {
        int r;
        struct pack_data pk;
        struct tdata td;
        if((r = recv_pack(fd,&pk)) < 0){
            printf("Client[%d] disconnected",fd);
            close(fd);
            pthread_exit(EXIT_SUCCESS);
        }

        struct tbyte tb;
        memcpy(tb.byte,pk.data,pk.len);
        tbyte2tdata(&tb,&td);

        pthread_mutex_lock(m);
        if(tpdb_report(&td) == 0){
            printf("insert: [%s][%s][%s]\n",td.mac,td.dtime,td.temp);
        }
        pthread_mutex_unlock(m);
    }
}

