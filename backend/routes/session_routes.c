#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "mongoose.h"
#include "routes.h"
#include "services.h"
#include "utils.h"

void handle_save_session(struct mg_connection *c, struct mg_http_message *hm) {
	char *body = dup_mg_string(hm->body);
	cJSON *req = body ? cJSON_Parse(body) : NULL;
	cJSON *game_code = req ? cJSON_GetObjectItem(req, "game_code") : NULL;
	cJSON *player_name = req ? cJSON_GetObjectItem(req, "player_name") : NULL;
	cJSON *state = req ? cJSON_GetObjectItem(req, "state") : NULL;

	if (!req || !cJSON_IsString(game_code) || !cJSON_IsString(player_name) || !cJSON_IsString(state)) {
		char *err = json_error("game_code, player_name, and state are required and must be strings");
		respond_json(c, 400, err);
		free(err);
		cJSON_Delete(req);
		free(body);
		return;
	}

	if (session_save(game_code->valuestring, player_name->valuestring, state->valuestring) != 0) {
		char *err = json_error("save_session_failed");
		respond_json(c, 500, err);
		free(err);
	} else {
		respond_json(c, 200, "{\"ok\":true}");
	}

	cJSON_Delete(req);
	free(body);
}

void handle_get_session(struct mg_connection *c, struct mg_http_message *hm) {
	char code[64] = {0};
	char encoded_name[128] = {0};
	char *player_name = NULL;
	char *state_json = NULL;
	cJSON *resp;
	char *out;

	/* URI is /session/<game_code>/<player_name> */
	/* length of "/session/" is 9 */
	if (hm->uri.len <= 9) {
		char *err = json_error("invalid_uri");
		respond_json(c, 400, err);
		free(err);
		return;
	}

	const char *remainder = hm->uri.buf + 9;
	int remainder_len = (int)hm->uri.len - 9;

	/* Find slash */
	const char *slash = NULL;
	for (int i = 0; i < remainder_len; i++) {
		if (remainder[i] == '/') {
			slash = remainder + i;
			break;
		}
	}

	if (!slash) {
		char *err = json_error("invalid_session_path");
		respond_json(c, 400, err);
		free(err);
		return;
	}

	int code_len = (int)(slash - remainder);
	int name_len = remainder_len - code_len - 1;

	if (code_len >= (int)sizeof(code) || name_len >= (int)sizeof(encoded_name) || code_len <= 0 || name_len <= 0) {
		char *err = json_error("path_params_too_long");
		respond_json(c, 400, err);
		free(err);
		return;
	}

	snprintf(code, sizeof(code), "%.*s", code_len, remainder);
	snprintf(encoded_name, sizeof(encoded_name), "%.*s", name_len, slash + 1);

	player_name = url_decode(encoded_name);
	if (!player_name) {
		char *err = json_error("decode_failed");
		respond_json(c, 400, err);
		free(err);
		return;
	}

	state_json = session_fetch(code, player_name);
	if (!state_json) {
		char *err = json_error("session_not_found");
		respond_json(c, 404, err);
		free(err);
		free(player_name);
		return;
	}

	resp = cJSON_CreateObject();
	cJSON_AddBoolToObject(resp, "ok", 1);
	cJSON_AddStringToObject(resp, "state", state_json);
	out = cJSON_PrintUnformatted(resp);
	respond_json(c, 200, out);

	cJSON_free(out);
	cJSON_Delete(resp);
	free(state_json);
	free(player_name);
}
