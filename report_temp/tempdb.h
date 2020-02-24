//
// Created by virgil on 2/12/20.
//

#ifndef UNTITLED1_TEMPDB_H
#define UNTITLED1_TEMPDB_H

#include <sqlite3.h>
#include <stdint.h>
#include "tdata.h"

#define LEN_SQL_STATE 256
#define DB_NAME "database.sqlite"
#define DB_PATH "./"DB_NAME
#define TB_NAME "tb_temperature"

struct RECORD{
    char machineid[7];      //PIXXXX
    char date[20];          //2020-07-02S12:00:00
    char temperature[5];    //XX.X
};

static sqlite3 *db;
static char *err_msg = 0;

static int callback(void *NotUsed, int argc, char **argv,char **azColName);
int tpdb_open(void);
int tpdb_report(struct tdata* td);
int tpdb_clear_all(void);
void tpdb_close(void);
int read_rec();

#endif //UNTITLED1_TEMPDB_H
