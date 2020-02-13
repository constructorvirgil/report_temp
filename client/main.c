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

#define LEN_MAC 6
#define LEN_DATETIME 19
#define LEN_TEMP 4


char machine_id[6]="PI0001";
char send_buf[30] = {0};
char temp_file_path[] = "/home/virgil/report_temp/client/temp.txt";

int get_datetime_now(char* datetime,int size)
{
    time_t t ;
    t= time(NULL);
    struct tm* tmp = localtime(&t);
    if(tmp == NULL){
        perror("localtime");
        exit(1);
    }
    strftime(datetime, size, "%F %T", tmp);
    return 0;
}

int get_temp(char* path,char* temp,int size)
{
    FILE* f = open(path,O_RDONLY);
    if(f == 0){
        perror("File open failed!\n");
        exit(EXIT_FAILURE);
    }
    int ret = read(f,temp,size);
    if(ret < 0){
        perror("Read file failed!\n");
        close(f);
        exit(EXIT_FAILURE);
    }
    close(f);
    return 0;
}

int send_temp(int fd)
{
    char datetime[128] ={0};
    char temp[4] = {0};
    char flag = 0xde;
    int size = 1+LEN_MAC+LEN_DATETIME+LEN_TEMP;

    get_datetime_now(datetime,size);
    get_temp(temp_file_path,temp,4);
    sprintf(send_buf,"%c%s%s%s%c",flag,machine_id,datetime,temp,size);

    int n = write(fd,send_buf,size);
    if(n < 0){
        return -1;
    }
    while(n < size){
        int new_n = write(fd,send_buf+n,size-n);
        if(new_n < 0){
            return -1;
        }
        n -= new_n;
    }
    return 0;
}


int main(int argc, char **argv)
{
    if(argc < 1){
        printf("too few arguments!\n");
        return -1;
    }

    int                     conn_fd = -1;
    struct sockaddr_in      serv_addr;
    char                    buf[1024];
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
        int ret = send_temp(conn_fd);
        printf("%s\n",send_buf);
        if(ret < 0){
            perror("Cannot connect server!\n");
            close(conn_fd);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    close(conn_fd);
}