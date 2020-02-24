//
// Created by virgil on 2/22/20.
//

#ifndef REPORT_TEMP_PACKER_H
#define REPORT_TEMP_PACKER_H

#include <stdint.h>
#define PACK_HEAD 0xde
#define PACK_DATA_MAXSIZE 256


struct pack_data{
    uint8_t data[PACK_DATA_MAXSIZE];
    uint8_t len;
};

int readn(int fd,uint8_t* buf,int size);
int sendn(int fd,uint8_t* buf,int size);
int send_pack(int fd,struct pack_data* pk);
int recv_pack(int fd,struct pack_data* pk);

#endif //REPORT_TEMP_PACKER_H
