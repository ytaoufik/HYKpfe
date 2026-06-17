#ifndef HYK_DB_H
#define HYK_DB_H

#include <sqlite3.h>

int db_init(const char *path);
void db_close(void);
sqlite3 *db_handle(void);
int db_exec_file(const char *file_path);
int db_run_migrations(void);

#endif
