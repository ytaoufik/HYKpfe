#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "utils.h"

typedef struct Buffer {
	char *data;
	size_t len;
} Buffer;

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	Buffer *mem = (Buffer *) userp;
	char *ptr = realloc(mem->data, mem->len + realsize + 1);

	if (!ptr) {
		return 0;
	}

	mem->data = ptr;
	memcpy(&(mem->data[mem->len]), contents, realsize);
	mem->len += realsize;
	mem->data[mem->len] = '\0';
	return realsize;
}

static char *extract_content_json(const char *response) {
	cJSON *root, *candidates, *first, *content, *parts, *first_part, *text;
	char *raw_json = NULL;

	root = cJSON_Parse(response);
	if (!root) {
		return NULL;
	}

	candidates = cJSON_GetObjectItem(root, "candidates");
	if (!cJSON_IsArray(candidates) || cJSON_GetArraySize(candidates) == 0) {
		cJSON_Delete(root);
		return NULL;
	}

	first = cJSON_GetArrayItem(candidates, 0);
	content = cJSON_GetObjectItem(first, "content");
	parts = content ? cJSON_GetObjectItem(content, "parts") : NULL;
	if (!cJSON_IsArray(parts) || cJSON_GetArraySize(parts) == 0) {
		cJSON_Delete(root);
		return NULL;
	}

	first_part = cJSON_GetArrayItem(parts, 0);
	text = first_part ? cJSON_GetObjectItem(first_part, "text") : NULL;
	if (cJSON_IsString(text) && text->valuestring) {
		raw_json = strdup(text->valuestring);
	}

	cJSON_Delete(root);
	return raw_json;
}

static char *extract_json_object(const char *text) {
	const char *start = strchr(text, '{');
	const char *end = strrchr(text, '}');
	char *out;
	int len;

	if (!start || !end || end <= start) {
		return NULL;
	}

	len = (int) (end - start + 1);
	out = malloc((size_t) len + 1);
	if (!out) {
		return NULL;
	}

	memcpy(out, start, (size_t) len);
	out[len] = '\0';
	return out;
}

static char *build_gemini_payload(const char *input_text) {
	cJSON *root, *contents, *parts, *part, *generation_config;
	char prompt[8192];
	char *payload;

	snprintf(prompt, sizeof(prompt),
			 "Create exactly 10 multiple-choice quiz questions from this lesson text. "
			 "Return only valid JSON with this exact shape: "
			 "{\"title\":\"...\",\"questions\":[{\"question\":\"...\",\"options\":[\"A\",\"B\",\"C\",\"D\"],\"correct_answer\":0,\"explanation\":\"...\"}]}. "
			 "Use real educational questions that clearly test the lesson content. "
			 "Lesson text: %s",
			 input_text);

	root = cJSON_CreateObject();
	contents = cJSON_AddArrayToObject(root, "contents");
	parts = cJSON_CreateArray();
	part = cJSON_CreateObject();
	cJSON_AddStringToObject(part, "text", prompt);
	cJSON_AddItemToArray(parts, part);

	{
		cJSON *content = cJSON_CreateObject();
		cJSON_AddItemToArray(contents, content);
		cJSON_AddItemToObject(content, "parts", parts);
	}

	generation_config = cJSON_AddObjectToObject(root, "generationConfig");
	cJSON_AddNumberToObject(generation_config, "temperature", 0.6);
	cJSON_AddNumberToObject(generation_config, "maxOutputTokens", 2200);
	cJSON_AddStringToObject(generation_config, "responseMimeType", "application/json");

	payload = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return payload;
}

char *gemini_generate_quiz_json(const char *input_text, int retries) {
	const char *key = getenv("GEMINI_API_KEY");
	const char *model = getenv("GEMINI_MODEL");
	char url[1024];
	char auth[512];
	char *payload;
	int attempt;

	if (!model || model[0] == '\0') {
		model = "gemini-1.5-flash";
	}

	if (!key || key[0] == '\0' || strcmp(key, "replace_with_your_real_gemini_key") == 0) {
		log_error("GEMINI_API_KEY not configured");
		return NULL;
	}

	if (retries < 0) {
		retries = 0;
	}

	snprintf(url, sizeof(url),
			 "https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s",
			 model, key);
	snprintf(auth, sizeof(auth), "Content-Type: application/json");
	payload = build_gemini_payload(input_text);
	if (!payload) {
		return NULL;
	}

	for (attempt = 0; attempt <= retries; attempt++) {
		CURL *curl = curl_easy_init();
		struct curl_slist *headers = NULL;
		Buffer response = {0};
		CURLcode res;
		long status = 0;

		if (!curl) {
			free(payload);
			return NULL;
		}

		headers = curl_slist_append(headers, auth);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 20000L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res == CURLE_OK && status >= 200 && status < 300) {
			char *response_text = response.data ? response.data : NULL;
			char *content = response_text ? extract_content_json(response_text) : NULL;
			char *json = content ? extract_json_object(content) : NULL;

			free(response.data);
			free(payload);
			free(content);
			if (json) {
				return json;
			}
			return NULL;
		}

		log_error("Gemini call failed attempt=%d status=%ld", attempt + 1, status);
		free(response.data);
	}

	free(payload);
	return NULL;
}
