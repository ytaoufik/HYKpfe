#include <stdlib.h>

#include "cJSON.h"
#include "utils.h"

char *json_error(const char *message) {
	cJSON *root = cJSON_CreateObject();
	char *out;

	if (!root) {
		return NULL;
	}
	cJSON_AddBoolToObject(root, "ok", 0);
	cJSON_AddStringToObject(root, "error", message ? message : "unknown_error");
	out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return out;
}
