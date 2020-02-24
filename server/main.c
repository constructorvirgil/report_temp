#include <stdio.h>
#include "tempdb.h"
#include "simple_server.h"
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
