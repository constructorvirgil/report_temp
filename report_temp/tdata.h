//
// Created by virgil on 2/23/20.
//

#ifndef REPORT_TEMP_TDATA_H
#define REPORT_TEMP_TDATA_H

#include <stdint.h>

#define L_MAC 6
#define L_DTIME 19
#define L_TEMP 4

struct mac_context{
    char id[7];
    char tpath[256];
};

struct tdata{
    char mac[L_MAC+1];
    char dtime[L_DTIME+1];
    char temp[L_TEMP+1];
};

struct tbyte{
    uint8_t byte[L_MAC+L_DTIME+L_TEMP];
};

int get_data(struct mac_context* m, struct tdata* td);
void tdata2tbyte(struct tdata* td,struct tbyte* tb);
void tbyte2tdata(struct tbyte* tb,struct tdata* td);

#endif //REPORT_TEMP_TDATA_H
