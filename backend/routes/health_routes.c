#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "mongoose.h"
#include "routes.h"
#include "services.h"
#include "utils.h"

void handle_health(struct mg_connection *c) {
	respond_json(c, 200, "{\"ok\":true,\"service\":\"hyk-backend\"}");
}

void handle_stats(struct mg_connection *c) {
	char *stats = stats_fetch_json();
	respond_json(c, 200, stats);
	cJSON_free(stats);
}
