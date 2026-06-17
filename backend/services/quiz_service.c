#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"
#include "db.h"
#include "services.h"
#include "utils.h"

char *quiz_generate_code(void) {
	static const char alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
	char *code = (char *)malloc(8);
	int i;

	if (!code) {
		return NULL;
	}
	srand((unsigned int)(time(NULL) ^ clock()));
	for (i = 0; i < 6; i++) {
		code[i] = alphabet[rand() % (int)(sizeof(alphabet) - 1)];
	}
	code[6] = '\0';
	return code;
}

bool quiz_validate_payload(const char *quiz_json, char *error_buf, int error_buf_len) {
	cJSON *root = cJSON_Parse(quiz_json);
	cJSON *title;
	cJSON *questions;
	int i;

	if (!root) {
		snprintf(error_buf, error_buf_len, "invalid_json");
		return false;
	}

	title = cJSON_GetObjectItem(root, "title");
	questions = cJSON_GetObjectItem(root, "questions");
	if (!cJSON_IsString(title) || !cJSON_IsArray(questions) || cJSON_GetArraySize(questions) != 10) {
		snprintf(error_buf, error_buf_len, "invalid_structure");
		cJSON_Delete(root);
		return false;
	}

	for (i = 0; i < 10; i++) {
		cJSON *q = cJSON_GetArrayItem(questions, i);
		cJSON *question = cJSON_GetObjectItem(q, "question");
		cJSON *options = cJSON_GetObjectItem(q, "options");
		cJSON *correct_answer = cJSON_GetObjectItem(q, "correct_answer");
		cJSON *explanation = cJSON_GetObjectItem(q, "explanation");

		if (!cJSON_IsString(question) || !cJSON_IsArray(options) || cJSON_GetArraySize(options) != 4 ||
			!cJSON_IsNumber(correct_answer) || correct_answer->valueint < 0 || correct_answer->valueint > 3 ||
			!cJSON_IsString(explanation)) {
			snprintf(error_buf, error_buf_len, "invalid_question_%d", i + 1);
			cJSON_Delete(root);
			return false;
		}
	}

	cJSON_Delete(root);
	return true;
}

int quiz_save(const char *game_code, const char *quiz_json) {
	sqlite3_stmt *stmt;
	const char *sql =
		"INSERT INTO quizzes(game_code, title, payload_json, created_at) "
		"VALUES(?, json_extract(?, '$.title'), ?, datetime('now'));";
	int rc;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		return -1;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, quiz_json, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, quiz_json, -1, SQLITE_TRANSIENT);

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return (rc == SQLITE_DONE) ? 0 : -1;
}

char *quiz_fetch_by_code(const char *game_code) {
	sqlite3_stmt *stmt;
	const char *sql = "SELECT payload_json FROM quizzes WHERE game_code=? LIMIT 1;";
	int rc;
	char *result = NULL;

	rc = sqlite3_prepare_v2(db_handle(), sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		return NULL;
	}

	sqlite3_bind_text(stmt, 1, game_code, -1, SQLITE_TRANSIENT);
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char *txt = sqlite3_column_text(stmt, 0);
		if (txt) {
			result = strdup((const char *)txt);
		}
	}
	sqlite3_finalize(stmt);
	return result;
}

char *stats_fetch_json(void) {
	sqlite3_stmt *stmt;
	cJSON *root = cJSON_CreateObject();
	char *out;
	int total_quizzes = 0;
	int total_players = 0;
	int total_scores = 0;

	sqlite3_prepare_v2(db_handle(), "SELECT COUNT(*) FROM quizzes;", -1, &stmt, NULL);
	if (sqlite3_step(stmt) == SQLITE_ROW) total_quizzes = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare_v2(db_handle(), "SELECT COUNT(*) FROM players;", -1, &stmt, NULL);
	if (sqlite3_step(stmt) == SQLITE_ROW) total_players = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare_v2(db_handle(), "SELECT COUNT(*) FROM scores;", -1, &stmt, NULL);
	if (sqlite3_step(stmt) == SQLITE_ROW) total_scores = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	cJSON_AddBoolToObject(root, "ok", 1);
	cJSON_AddNumberToObject(root, "quizzes", total_quizzes);
	cJSON_AddNumberToObject(root, "players", total_players);
	cJSON_AddNumberToObject(root, "scores", total_scores);
	out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return out;
}
