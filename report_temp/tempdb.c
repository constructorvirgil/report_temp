//
// Created by virgil on 2/12/20.
//

#include "tempdb.h"
#include "stdio.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

int get_datetime_now(char* datetime,int size)
{
    time_t t ;
    t= time(NULL);
    struct tm* tmp = localtime(&t);
    if(tmp == NULL){
        perror("localtime");
        exit(EXIT_FAILURE);
    }
    strftime(datetime, size, "%F %T", tmp);
    return 0;
}

static int callback(void *NotUsed, int argc, char **argv,char **azColName) {
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int read_rec()
{
    int rc = 0;
    char sql[] = "SELECT * FROM temp_rec;";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(TPDB_ERROR);
    }
    return 0;
}

int tpdb_open(void)
{
    int rc = 0;
    char sql[LEN_SQL_SATE] = {0};

    //open database
    rc = sqlite3_open_v2("temp_rec.db", &db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(TPDB_ERROR);
    }

    strcpy(sql,"CREATE TABLE temp_rec(mac CHAR(6),time DATETIME, temp DOUBLE); ");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK && rc != 1 ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(TPDB_ERROR);
    }

    return 0;
}

int tpdb_report(char* mac,char* datetime,char* temp)
{

    int rc = 0;
    char sql[LEN_SQL_SATE] = {0};

    sprintf(sql,"INSERT INTO %s (mac,time,temp) VALUES (\"%s\",\"%s\",%s);","temp_rec",mac,datetime,temp);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    return 0;
}

int tpdb_clear_all(void)
{
    int rc = 0;
    char sql[256] = {0};

    sprintf(sql,"DELETE FROM temp_rec;");
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(TPDB_ERROR);
    }
    return 0;
}

void tpdb_close(void)
{
    sqlite3_close(db);
}


