//
// Created by virgil on 2/12/20.
//

#ifndef UNTITLED1_TEMPDB_H
#define UNTITLED1_TEMPDB_H

#include <sqlite3.h>


#define LEN_MAC 6
#define LEN_DATETIME 19
#define LEN_TEMP 4
#define LEN_SQL_SATE 256

#define TPDB_ERROR 1

static sqlite3 *db;
static char *err_msg = 0;

int get_datetime_now(char* datetime,int size);
int tpdb_open(void);
int tpdb_report(char* mac,char* datetime,char* temp);
int tpdb_clear_all(void);
void tpdb_close(void);
int read_rec();

#endif //UNTITLED1_TEMPDB_H
