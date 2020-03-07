#include <stdio.h>
#include "tempdb.h"
//#include "multipro_server.h"
//#include "multithr_server.h"
#include "sel_server.h"
#include "packer.h"


int main() {
    //PI00012020-02-22 20:01:27 23.6
    //char data[] = "PI00012020-02-22 20:01:2723.6";

    tpdb_open();
    //tpdb_clear_all();
    server_init(9100);
    //read_rec();
    tpdb_close();

    return 0;
}
