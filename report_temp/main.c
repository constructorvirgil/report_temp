#include <stdio.h>
#include "tempdb.h"


int main() {
    tpdb_open();
    tpdb_clear_all();
    tpdb_report("PI0001","32.8");
    read_rec();

    tpdb_close();

    return 0;
}
