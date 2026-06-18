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
	Buffer *mem = (Buffer *)userp;
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

static char *extract_content_from_openai(const char *response) {
	cJSON *root = cJSON_Parse(response);
	cJSON *choices, *first, *message, *content;
	char *result = NULL;

	if (!root) {
		return NULL;
	}

	choices = cJSON_GetObjectItem(root, "choices");
	if (!cJSON_IsArray(choices) || cJSON_GetArraySize(choices) == 0) {
		cJSON_Delete(root);
		return NULL;
	}

	first = cJSON_GetArrayItem(choices, 0);
	message = first ? cJSON_GetObjectItem(first, "message") : NULL;
	content = message ? cJSON_GetObjectItem(message, "content") : NULL;

	if (cJSON_IsString(content) && content->valuestring) {
		result = strdup(content->valuestring);
	}

	cJSON_Delete(root);
	return result;
}

static char *extract_json_object(const char *text) {
	const char *start = strchr(text, '{');
	const char *end = strrchr(text, '}');
	char *out;
	int len;

	if (!start || !end || end <= start) {
		return NULL;
	}

	len = (int)(end - start + 1);
	out = malloc((size_t)len + 1);
	if (!out) {
		return NULL;
	}

	memcpy(out, start, (size_t)len);
	out[len] = '\0';
	return out;
}

static char *build_openai_payload(const char *input_text) {
	cJSON *root, *messages, *sys_msg, *user_msg;
	const char *model = getenv("OPENAI_MODEL");
	char user_prompt[8192];
	char *payload;

	if (!model || model[0] == '\0') {
		model = "gpt-4o-mini";
	}

	snprintf(user_prompt, sizeof(user_prompt),
			 "Create exactly 10 multiple-choice quiz questions from this lesson content. "
			 "Return ONLY valid JSON with this exact structure: "
			 "{\"title\":\"...\",\"questions\":[{\"question\":\"...\",\"options\":[\"A\",\"B\",\"C\",\"D\"],\"correct_answer\":0,\"explanation\":\"...\"}]}. "
			 "Each question must have exactly 4 options. correct_answer is the 0-based index of the correct option. "
			 "Generate educational questions that clearly test understanding of the content. "
			 "Lesson content: %s",
			 input_text);

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "model", model);
	cJSON_AddNumberToObject(root, "temperature", 0.6);
	cJSON_AddNumberToObject(root, "max_tokens", 2500);

	messages = cJSON_AddArrayToObject(root, "messages");

	sys_msg = cJSON_CreateObject();
	cJSON_AddStringToObject(sys_msg, "role", "system");
	cJSON_AddStringToObject(sys_msg, "content",
		"You are a quiz generator. You MUST return ONLY valid JSON, no markdown, no explanation, no code fences. "
		"The JSON must contain a \"title\" string and a \"questions\" array with exactly 10 objects.");
	cJSON_AddItemToArray(messages, sys_msg);

	user_msg = cJSON_CreateObject();
	cJSON_AddStringToObject(user_msg, "role", "user");
	cJSON_AddStringToObject(user_msg, "content", user_prompt);
	cJSON_AddItemToArray(messages, user_msg);

	payload = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return payload;
}

char *openai_generate_quiz_json(const char *input_text, int retries) {
	const char *key = getenv("OPENAI_API_KEY");
	char auth[512];
	char *payload;
	int attempt;
	int backoff_ms = 1000;

	if (!key || key[0] == '\0' || strcmp(key, "your_openai_api_key") == 0) {
		log_error("OPENAI_API_KEY not configured");
		return NULL;
	}

	if (retries < 0) {
		retries = 0;
	}

	snprintf(auth, sizeof(auth), "Authorization: Bearer %s", key);
	payload = build_openai_payload(input_text);
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

		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, auth);

		curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res == CURLE_OK && status >= 200 && status < 300 && response.data) {
			char *content = extract_content_from_openai(response.data);
			char *json = content ? extract_json_object(content) : NULL;

			free(response.data);
			free(content);

			if (json) {
				free(payload);
				return json;
			}

			log_error("OpenAI returned unparseable content on attempt=%d", attempt + 1);
		} else {
			log_error("OpenAI call failed attempt=%d curl=%d status=%ld", attempt + 1, (int)res, status);
			free(response.data);
		}

		if (attempt < retries) {
#if defined(_WIN32)
			Sleep(backoff_ms);
#else
			usleep(backoff_ms * 1000);
#endif
			backoff_ms *= 2;
		}
	}

	free(payload);
	return NULL;
}
