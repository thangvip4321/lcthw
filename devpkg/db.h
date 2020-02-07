#ifndef db_h
#define db_h

#define DB_FILE "/usr/local/.devpkg/db"
#define DB_DIR "/usr/local/.devpkg"
int DB_list();
int DB_init();
int DB_update(const char *url);
int DB_find(const char *url);
#endif