#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "mongoose.h"
#include "routes.h"
#include "services.h"
#include "utils.h"

void handle_submit_score(struct mg_connection *c, struct mg_http_message *hm) {
	char *body = dup_mg_string(hm->body);
	if (!body || score_submit(body) != 0) {
		char *err = json_error("submit_score_failed");
		respond_json(c, 400, err);
		free(err);
	} else {
		respond_json(c, 200, "{\"ok\":true}");
	}
	free(body);
}

void handle_leaderboard(struct mg_connection *c, struct mg_http_message *hm) {
	char code[64] = {0};
	char *json;

	/* URI is /leaderboard/<game_code> */
	/* length of "/leaderboard/" is 13 */
	if (hm->uri.len <= 13) {
		char *err = json_error("invalid_uri");
		respond_json(c, 400, err);
		free(err);
		return;
	}

	snprintf(code, sizeof(code), "%.*s", (int)hm->uri.len - 13, hm->uri.buf + 13);
	json = leaderboard_fetch(code);
	if (!json) {
		char *err = json_error("leaderboard_failed");
		respond_json(c, 500, err);
		free(err);
		return;
	}
	respond_json(c, 200, json);
	cJSON_free(json);
}
