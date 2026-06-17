#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "mongoose.h"

#include "app.h"
#include "services.h"
#include "utils.h"

static AppContext *g_ctx = NULL;

static int method_is(struct mg_http_message *hm, const char *method) {
	size_t len = strlen(method);
	return hm->method.len == len && strncmp(hm->method.buf, method, len) == 0;
}

static int uri_matches(struct mg_http_message *hm, const char *pattern) {
	return mg_match(hm->uri, mg_str(pattern), NULL);
}

static char *dup_mg_string(struct mg_str s) {
	char *out = (char *)malloc((size_t)s.len + 1);
	if (!out) {
		return NULL;
	}
	memcpy(out, s.buf, (size_t)s.len);
	out[s.len] = '\0';
	return out;
}

static void respond_json(struct mg_connection *c, int status, const char *json_body) {
	mg_http_reply(c, status,
				  "Content-Type: application/json\r\n"
				  "Access-Control-Allow-Origin: *\r\n"
				  "Access-Control-Allow-Headers: Content-Type\r\n",
				  "%s", json_body ? json_body : "{}");
}

static void handle_generate_quiz(struct mg_connection *c, struct mg_http_message *hm) {
	char *body = dup_mg_string(hm->body);
	cJSON *req = body ? cJSON_Parse(body) : NULL;
	cJSON *input = req ? cJSON_GetObjectItem(req, "input") : NULL;
	char validation_error[128] = {0};
	char *quiz_json;
	char *code;
	cJSON *resp;
	char *out;

	if (!req || !cJSON_IsString(input)) {
		char *err = json_error("input is required");
		respond_json(c, 400, err);
		free(err);
		cJSON_Delete(req);
		free(body);
		return;
	}

	quiz_json = gemini_generate_quiz_json(input->valuestring, g_ctx->config.openai_max_retries);
	if (!quiz_json) {
		char *err = json_error("gemini_failed");
		respond_json(c, 502, err);
		free(err);
		cJSON_Delete(req);
		free(body);
		return;
	}

	if (!quiz_validate_payload(quiz_json, validation_error, sizeof(validation_error))) {
		char *err = json_error(validation_error);
		respond_json(c, 422, err);
		free(err);
		free(quiz_json);
		cJSON_Delete(req);
		free(body);
		return;
	}

	code = quiz_generate_code();
	if (!code || quiz_save(code, quiz_json) != 0) {
		char *err = json_error("save_quiz_failed");
		respond_json(c, 500, err);
		free(err);
		free(code);
		free(quiz_json);
		cJSON_Delete(req);
		free(body);
		return;
	}

	resp = cJSON_CreateObject();
	cJSON_AddBoolToObject(resp, "ok", 1);
	cJSON_AddStringToObject(resp, "game_code", code);
	cJSON_AddItemToObject(resp, "quiz", cJSON_Parse(quiz_json));
	out = cJSON_PrintUnformatted(resp);

	respond_json(c, 200, out);

	cJSON_free(out);
	cJSON_Delete(resp);
	free(code);
	free(quiz_json);
	cJSON_Delete(req);
	free(body);
}

static void handle_save_quiz(struct mg_connection *c, struct mg_http_message *hm) {
	char *body = dup_mg_string(hm->body);
	cJSON *req = body ? cJSON_Parse(body) : NULL;
	cJSON *game_code = req ? cJSON_GetObjectItem(req, "game_code") : NULL;
	cJSON *quiz = req ? cJSON_GetObjectItem(req, "quiz") : NULL;
	char *quiz_json;
	char validation_error[128] = {0};

	if (!req || !cJSON_IsString(game_code) || !cJSON_IsObject(quiz)) {
		char *err = json_error("game_code and quiz are required");
		respond_json(c, 400, err);
		free(err);
		cJSON_Delete(req);
		free(body);
		return;
	}

	quiz_json = cJSON_PrintUnformatted(quiz);
	if (!quiz_validate_payload(quiz_json, validation_error, sizeof(validation_error))) {
		char *err = json_error(validation_error);
		respond_json(c, 422, err);
		free(err);
		cJSON_free(quiz_json);
		cJSON_Delete(req);
		free(body);
		return;
	}

	if (quiz_save(game_code->valuestring, quiz_json) != 0) {
		char *err = json_error("save_failed");
		respond_json(c, 500, err);
		free(err);
	} else {
		respond_json(c, 200, "{\"ok\":true}");
	}

	cJSON_free(quiz_json);
	cJSON_Delete(req);
	free(body);
}

static void handle_get_quiz(struct mg_connection *c, struct mg_http_message *hm) {
	char code[64] = {0};
	char *quiz_json;
	cJSON *resp;
	char *out;

	snprintf(code, sizeof(code), "%.*s", (int)hm->uri.len - 6, hm->uri.buf + 6);

	quiz_json = quiz_fetch_by_code(code);
	if (!quiz_json) {
		char *err = json_error("quiz_not_found");
		respond_json(c, 404, err);
		free(err);
		return;
	}

	resp = cJSON_CreateObject();
	cJSON_AddBoolToObject(resp, "ok", 1);
	cJSON_AddStringToObject(resp, "game_code", code);
	cJSON_AddItemToObject(resp, "quiz", cJSON_Parse(quiz_json));
	out = cJSON_PrintUnformatted(resp);
	respond_json(c, 200, out);

	cJSON_free(out);
	cJSON_Delete(resp);
	free(quiz_json);
}

static void handle_submit_score(struct mg_connection *c, struct mg_http_message *hm) {
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

static void handle_leaderboard(struct mg_connection *c, struct mg_http_message *hm) {
	char code[64] = {0};
	char *json;

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

static void handle_health(struct mg_connection *c) {
	respond_json(c, 200, "{\"ok\":true,\"service\":\"hyk-backend\"}");
}

static void handle_stats(struct mg_connection *c) {
	char *stats = stats_fetch_json();
	respond_json(c, 200, stats);
	cJSON_free(stats);
}

static void event_handler(struct mg_connection *c, int ev, void *ev_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message *hm = (struct mg_http_message *)ev_data;

		if (method_is(hm, "OPTIONS")) {
			mg_http_reply(c, 204,
						  "Access-Control-Allow-Origin: *\r\n"
						  "Access-Control-Allow-Headers: Content-Type\r\n"
						  "Access-Control-Allow-Methods: GET,POST,OPTIONS\r\n", "");
			return;
		}

		if (uri_matches(hm, "/generate-quiz") && method_is(hm, "POST")) {
			handle_generate_quiz(c, hm);
		} else if (uri_matches(hm, "/save-quiz") && method_is(hm, "POST")) {
			handle_save_quiz(c, hm);
		} else if (uri_matches(hm, "/quiz/*") && method_is(hm, "GET")) {
			handle_get_quiz(c, hm);
		} else if (uri_matches(hm, "/submit-score") && method_is(hm, "POST")) {
			handle_submit_score(c, hm);
		} else if (uri_matches(hm, "/leaderboard/*") && method_is(hm, "GET")) {
			handle_leaderboard(c, hm);
		} else if (uri_matches(hm, "/health") && method_is(hm, "GET")) {
			handle_health(c);
		} else if (uri_matches(hm, "/stats") && method_is(hm, "GET")) {
			handle_stats(c);
		} else {
			respond_json(c, 404, "{\"ok\":false,\"error\":\"not_found\"}");
		}
	}
}

int app_start_server(AppContext *ctx) {
	struct mg_mgr mgr;
	struct mg_connection *conn;
	char listen_addr[64];

	g_ctx = ctx;
	mg_mgr_init(&mgr);
	snprintf(listen_addr, sizeof(listen_addr), "http://0.0.0.0:%s", ctx->config.port);

	conn = mg_http_listen(&mgr, listen_addr, event_handler, NULL);
	if (!conn) {
		log_error("Failed to listen on %s", listen_addr);
		mg_mgr_free(&mgr);
		return 1;
	}

	log_info("Listening on %s", listen_addr);
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);
	return 0;
}
