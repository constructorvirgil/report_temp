#include <stdio.h>
#include "tempdb.h"
#include "simple_server.h"

int main() {

    tpdb_open();
    server_init(9100);
    //read_rec();
    tpdb_close();
    return 0;
}
