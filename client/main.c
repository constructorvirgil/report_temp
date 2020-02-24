

#include "tdata.h"
#include "packer.h"

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
#include <getopt.h>


char macid[7] = {0};
char tpath[256] = {0};
int port;

struct option long_options[] = {
        {"mac", required_argument, NULL, 'm'},
        {"path", required_argument, NULL, 'l'},
        {"port", required_argument, NULL, 'p'}
};

static void usage(void)
{
    fprintf(stderr,
            "client [option]\n"
            "  -p|--port <port>                    Connect to which port.\n"
            "  -m|--mac <machine_id>              Specify machine id.\n"
            "  -f|--path <path_of_temperature>     Specify where to get temperature.\n"
            "  -?|-h|--help                 This information.\n");
};

int main(int argc, char *argv[])
{
    int opt;
    int arg_count = 0;

    if (argc == 1)
    {
        usage();
        return 1;
    }

    while ((opt = getopt_long(argc, argv, "m:p:l:", long_options, NULL)) != EOF)
    {
        switch (opt)
        {
            case 0:
                break;
            case 'm':
                strcpy(macid,optarg);
                arg_count++;
                break;
            case 'p':
                port = atoi(optarg);
                arg_count++;
                break;
            case 'l':
                strcpy(tpath,optarg);
                arg_count++;
                break;
            case ':':
            case 'h':
            case '?':
            default:
                usage();
                return 2;
        }
    }

    if(arg_count != 3){
        usage();
        return 3;
    }


    printf("Machine ID      :%s\n",macid);
    printf("Port            :%d\n",port);
    printf("Tempfile Path   :%s\n",tpath);

    int                     conn_fd = -1;
    struct sockaddr_in      serv_addr;
    char                    buf[1024];

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
    {
        perror("Create socket failed!");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton( "127.0.0.1", &serv_addr.sin_addr );

    printf("connect to port %d...\n",port);

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Cannot connect to server!");
        close(conn_fd);
        exit(EXIT_FAILURE);
    }

    printf("connected successfully!\n");
    printf("trying to send message...\n");

    while(1)
    {
        int r;
        struct mac_context m;
        struct tdata td;
        struct tbyte tb;
        strcpy(m.id,macid);
        strcpy(m.tpath,tpath);

        if((r = get_data(&m,&td)) < 0){
            fprintf(stderr,"Get temperature failed!\n");
            close(conn_fd);
            return -1;
        }
        tdata2tbyte(&td,&tb);

        struct pack_data pk;
        memcpy(pk.data,tb.byte,sizeof(tb.byte));
        pk.len = sizeof(tb.byte);
        if((r = send_pack(conn_fd,&pk)) < 0){
            fprintf(stderr,"Disconnected with server\n");
            close(conn_fd);
            return -2;
        }
        printf("send:[%s][%s][%s]\n",td.mac,td.dtime,td.temp);

        sleep(1);
    }

    close(conn_fd);
}