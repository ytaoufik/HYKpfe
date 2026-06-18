#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "services.h"
#include "utils.h"

int session_save(const char *game_code, const char *player_name, const char *state_json) {
	sqlite3_stmt *stmt;
	const char *sql =
		"INSERT INTO sessions(game_code, player_name, state_json, updated_at) "
		"VALUES(?, ?, ?, datetime('now')) "
		"ON CONFLICT(game_code, player_name) DO UPDATE SET "
		"state_json=excluded.state_json, updated_at=excluded.updated_at;";
	int rc;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("Failed to prepare session_save statement: %s", sqlite3_errmsg(db_handle()));
		return -1;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, state_json, -1, SQLITE_TRANSIENT);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return (rc == SQLITE_DONE) ? 0 : -1;
}

char *session_fetch(const char *game_code, const char *player_name) {
	sqlite3_stmt *stmt;
	const char *sql = "SELECT state_json FROM sessions WHERE game_code=? AND player_name=? LIMIT 1;";
	int rc;
	char *result = NULL;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("Failed to prepare session_fetch statement: %s", sqlite3_errmsg(db_handle()));
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player_name, -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char *txt = sqlite3_column_text(stmt, 0);
		if (txt) {
			result = strdup((const char *)txt);
		}
	}

	sqlite3_finalize(stmt);
	return result;
}
