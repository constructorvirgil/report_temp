//
// Created by virgil on 2/23/20.
//

#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "tdata.h"

int get_data(struct mac_context* m, struct tdata* td)
{
    int r;
    time_t t ;
    struct tm* tmp;
    int f;

    //id
    strcpy(td->mac,m->id);

    //dtime
    t= time(NULL);
    if((tmp = localtime(&t)) == NULL){
        return -1;
    }
    strftime(td->dtime, L_DTIME+1, "%F %T", tmp);

    //temp
    if((f = open(m->tpath,O_RDONLY)) == -1){
        return -2;
    }
    if((r = read(f,td->temp,L_TEMP)) < 0){
        close(f);
        return -2;
    }
    td->temp[L_TEMP] = '\0';
    close(f);
    return 0;
}

void tdata2tbyte(struct tdata* td,struct tbyte* tb)
{
    memcpy(tb->byte,td->mac,L_MAC);
    memcpy(tb->byte+L_MAC,td->dtime,L_DTIME);
    memcpy(tb->byte+L_MAC+L_DTIME,td->temp,L_TEMP);
}

void tbyte2tdata(struct tbyte* tb,struct tdata* td)
{
    memcpy(td->mac,tb->byte,L_MAC);
    td->mac[L_MAC] = '\0';
    memcpy(td->dtime,tb->byte+L_MAC,L_DTIME);
    td->dtime[L_DTIME] = '\0';
    memcpy(td->temp,tb->byte+L_MAC+L_DTIME,L_TEMP);
    td->temp[L_TEMP] = '\0';
}