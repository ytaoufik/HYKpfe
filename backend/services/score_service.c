#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "db.h"
#include "services.h"

int score_submit(const char *body_json) {
	cJSON *root = cJSON_Parse(body_json);
	cJSON *code;
	cJSON *player;
	cJSON *score;
	cJSON *xp;
	cJSON *streak;
	cJSON *lives;
	sqlite3_stmt *stmt;
	int rc;

	if (!root) {
		return -1;
	}

	code = cJSON_GetObjectItem(root, "game_code");
	player = cJSON_GetObjectItem(root, "player_name");
	score = cJSON_GetObjectItem(root, "score");
	xp = cJSON_GetObjectItem(root, "xp");
	streak = cJSON_GetObjectItem(root, "streak");
	lives = cJSON_GetObjectItem(root, "lives");

	if (!cJSON_IsString(code) || !cJSON_IsString(player) || !cJSON_IsNumber(score) ||
		!cJSON_IsNumber(xp) || !cJSON_IsNumber(streak) || !cJSON_IsNumber(lives)) {
		cJSON_Delete(root);
		return -1;
	}

	rc = sqlite3_prepare_v2(db_handle(),
							"INSERT OR IGNORE INTO players(game_code, player_name, created_at) VALUES(?, ?, datetime('now'));",
							-1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cJSON_Delete(root);
		return -1;
	}
	sqlite3_bind_text(stmt, 1, code->valuestring, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player->valuestring, -1, SQLITE_TRANSIENT);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	rc = sqlite3_prepare_v2(db_handle(),
							"INSERT INTO scores(game_code, player_name, score, xp, streak, lives, submitted_at) "
							"VALUES(?, ?, ?, ?, ?, ?, datetime('now'));",
							-1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cJSON_Delete(root);
		return -1;
	}

	sqlite3_bind_text(stmt, 1, code->valuestring, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, player->valuestring, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, score->valueint);
	sqlite3_bind_int(stmt, 4, xp->valueint);
	sqlite3_bind_int(stmt, 5, streak->valueint);
	sqlite3_bind_int(stmt, 6, lives->valueint);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	cJSON_Delete(root);
	return (rc == SQLITE_DONE) ? 0 : -1;
}

char *leaderboard_fetch(const char *game_code) {
	sqlite3_stmt *stmt;
	cJSON *root = cJSON_CreateObject();
	cJSON *items = cJSON_CreateArray();
	char *out;
	int rc;

	rc = sqlite3_prepare_v2(db_handle(),
							"SELECT player_name, MAX(score) AS best_score, MAX(xp) AS best_xp "
							"FROM scores WHERE game_code=? GROUP BY player_name ORDER BY best_score DESC, best_xp DESC LIMIT 50;",
							-1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cJSON_Delete(root);
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		cJSON *row = cJSON_CreateObject();
		cJSON_AddStringToObject(row, "player_name", (const char *)sqlite3_column_text(stmt, 0));
		cJSON_AddNumberToObject(row, "score", sqlite3_column_int(stmt, 1));
		cJSON_AddNumberToObject(row, "xp", sqlite3_column_int(stmt, 2));
		cJSON_AddItemToArray(items, row);
	}
	sqlite3_finalize(stmt);

	cJSON_AddBoolToObject(root, "ok", 1);
	cJSON_AddStringToObject(root, "game_code", game_code);
	cJSON_AddItemToObject(root, "leaderboard", items);
	out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return out;
}
