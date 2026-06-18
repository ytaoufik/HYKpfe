#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "mongoose.h"

#include "app.h"
#include "routes.h"
#include "services.h"
#include "utils.h"

static AppContext *g_ctx = NULL;

AppContext *app_get_context(void) {
	return g_ctx;
}

static int method_is(struct mg_http_message *hm, const char *method) {
	size_t len = strlen(method);
	return hm->method.len == len && strncmp(hm->method.buf, method, len) == 0;
}

static int uri_matches(struct mg_http_message *hm, const char *pattern) {
	return mg_match(hm->uri, mg_str(pattern), NULL);
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
		} else if (uri_matches(hm, "/session/save") && method_is(hm, "POST")) {
			handle_save_session(c, hm);
		} else if (uri_matches(hm, "/session/*") && method_is(hm, "GET")) {
			handle_get_session(c, hm);
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
