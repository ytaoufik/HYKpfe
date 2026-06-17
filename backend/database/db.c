#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "utils.h"

static sqlite3 *g_db = NULL;

sqlite3 *db_handle(void) {
	return g_db;
}

int db_init(const char *path) {
	if (sqlite3_open(path, &g_db) != SQLITE_OK) {
		log_error("SQLite open error: %s", sqlite3_errmsg(g_db));
		return -1;
	}
	sqlite3_exec(g_db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
	sqlite3_exec(g_db, "PRAGMA journal_mode = WAL;", NULL, NULL, NULL);
	return 0;
}

void db_close(void) {
	if (g_db) {
		sqlite3_close(g_db);
		g_db = NULL;
	}
}

int db_exec_file(const char *file_path) {
	char *sql = read_file_text(file_path);
	char *err_msg = NULL;
	int rc;

	if (!sql) {
		log_error("Cannot read SQL file: %s", file_path);
		return -1;
	}

	rc = sqlite3_exec(g_db, sql, NULL, NULL, &err_msg);
	if (rc != SQLITE_OK) {
		log_error("SQLite exec error in %s: %s", file_path, err_msg ? err_msg : "unknown");
		sqlite3_free(err_msg);
		free(sql);
		return -1;
	}
	free(sql);
	return 0;
}

int db_run_migrations(void) {
	if (db_exec_file("./schema.sql") != 0) {
		return -1;
	}
	if (db_exec_file("./seed.sql") != 0) {
		return -1;
	}
	return 0;
}
