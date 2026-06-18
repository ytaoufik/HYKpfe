#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "db.h"
#include "services.h"
#include "utils.h"

int achievement_save(const char *game_code, const char *player_name, const char *badge_key) {
	sqlite3_stmt *stmt;
	const char *sql =
		"INSERT OR IGNORE INTO achievements(game_code, player_name, badge_key, unlocked_at) "
		"VALUES(?, ?, ?, datetime('now'));";
	int rc;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("Failed to prepare achievement_save statement: %s", sqlite3_errmsg(db_handle()));
		return -1;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player_name, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, badge_key, -1, SQLITE_TRANSIENT);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return (rc == SQLITE_DONE) ? 0 : -1;
}

char *achievements_fetch(const char *game_code, const char *player_name) {
	sqlite3_stmt *stmt;
	const char *sql = "SELECT badge_key FROM achievements WHERE game_code=? AND player_name=?;";
	int rc;
	cJSON *root = cJSON_CreateObject();
	cJSON *items = cJSON_CreateArray();
	char *out;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("Failed to prepare achievements_fetch statement: %s", sqlite3_errmsg(db_handle()));
		cJSON_Delete(root);
		cJSON_Delete(items);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player_name, -1, SQLITE_TRANSIENT);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char *badge = sqlite3_column_text(stmt, 0);
		if (badge) {
			cJSON_AddItemToArray(items, cJSON_CreateString((const char *)badge));
		}
	}
	sqlite3_finalize(stmt);

	cJSON_AddBoolToObject(root, "ok", 1);
	cJSON_AddStringToObject(root, "game_code", game_code);
	cJSON_AddStringToObject(root, "player_name", player_name);
	cJSON_AddItemToObject(root, "achievements", items);
	out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return out;
}
