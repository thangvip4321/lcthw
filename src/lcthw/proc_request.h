#ifndef _proc_request_
#define _proc_request_
#include <stdio.h>
#include <lcthw/tstree.h>
#include <mysql.h>
char* process_request(TSTree** root,char* commands,MYSQL* con);
void finish_with_error(MYSQL* con);
MYSQL* mysql_db_init();
#endif