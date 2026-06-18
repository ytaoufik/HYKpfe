#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "cJSON.h"
#include "mongoose.h"
#include "routes.h"
#include "services.h"
#include "utils.h"

void handle_generate_quiz(struct mg_connection *c, struct mg_http_message *hm) {
	char *body = dup_mg_string(hm->body);
	cJSON *req = body ? cJSON_Parse(body) : NULL;
	cJSON *input = req ? cJSON_GetObjectItem(req, "input") : NULL;
	char validation_error[128] = {0};
	char *quiz_json;
	char *code;
	cJSON *resp;
	char *out;
	AppContext *ctx = app_get_context();

	if (!req || !cJSON_IsString(input)) {
		char *err = json_error("input is required");
		respond_json(c, 400, err);
		free(err);
		cJSON_Delete(req);
		free(body);
		return;
	}

	quiz_json = ai_generate_quiz_json(input->valuestring, ctx ? ctx->config.openai_max_retries : 3);
	if (!quiz_json) {
		char *err = json_error("ai_generation_failed");
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

void handle_save_quiz(struct mg_connection *c, struct mg_http_message *hm) {
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

void handle_get_quiz(struct mg_connection *c, struct mg_http_message *hm) {
	char code[64] = {0};
	char *quiz_json;
	cJSON *resp;
	char *out;

	/* URI is /quiz/<game_code> */
	/* length of "/quiz/" is 6 */
	if (hm->uri.len <= 6) {
		char *err = json_error("invalid_uri");
		respond_json(c, 400, err);
		free(err);
		return;
	}

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
