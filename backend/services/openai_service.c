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

static char *generate_mock_quiz(const char *input_text) {
	cJSON *root, *questions, *q, *options;
	char *payload;
	int i;

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "title", input_text);

	questions = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "questions", questions);

	for (i = 0; i < 10; i++) {
		char qtext[256], optA[64], optB[64], optC[64], optD[64];

		q = cJSON_CreateObject();
		snprintf(qtext, sizeof(qtext), "Question %d about %s?", i + 1, input_text);
		cJSON_AddStringToObject(q, "question", qtext);

		options = cJSON_CreateArray();
		snprintf(optA, sizeof(optA), "Option A_%d", i + 1);
		snprintf(optB, sizeof(optB), "Option B_%d", i + 1);
		snprintf(optC, sizeof(optC), "Option C_%d (Correct)", i + 1);
		snprintf(optD, sizeof(optD), "Option D_%d", i + 1);
		cJSON_AddItemToArray(options, cJSON_CreateString(optA));
		cJSON_AddItemToArray(options, cJSON_CreateString(optB));
		cJSON_AddItemToArray(options, cJSON_CreateString(optC));
		cJSON_AddItemToArray(options, cJSON_CreateString(optD));

		cJSON_AddItemToObject(q, "options", options);
		cJSON_AddNumberToObject(q, "correct_answer", 2);
		cJSON_AddStringToObject(q, "explanation", "This is the correct answer.");

		cJSON_AddItemToArray(questions, q);
	}

	payload = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return payload;
}

static char *extract_content_json(const char *response) {
	cJSON *root = cJSON_Parse(response);
	cJSON *choices;
	cJSON *first;
	cJSON *msg;
	cJSON *content;
	char *raw_json = NULL;

	if (!root) {
		return NULL;
	}

	choices = cJSON_GetObjectItem(root, "choices");
	if (!cJSON_IsArray(choices) || cJSON_GetArraySize(choices) == 0) {
		cJSON_Delete(root);
		return NULL;
	}

	first = cJSON_GetArrayItem(choices, 0);
	msg = cJSON_GetObjectItem(first, "message");
	content = cJSON_GetObjectItem(msg, "content");
	if (cJSON_IsString(content) && content->valuestring) {
		raw_json = strdup(content->valuestring);
	}

	cJSON_Delete(root);
	return raw_json;
}

static char *build_openai_payload(const char *input_text, const char *model) {
	cJSON *root = cJSON_CreateObject();
	cJSON *messages = cJSON_CreateArray();
	cJSON *sys = cJSON_CreateObject();
	cJSON *usr = cJSON_CreateObject();
	char prompt[8192];
	char *payload;

	snprintf(prompt, sizeof(prompt),
			 "Generate exactly 10 MCQ questions from this source: %s\\n"
			 "Output strict JSON only, no markdown, format: "
			 "{\\\"title\\\":\\\"...\\\",\\\"questions\\\":[{\\\"question\\\":\\\"...\\\",\\\"options\\\":[\\\"A\\\",\\\"B\\\",\\\"C\\\",\\\"D\\\"],\\\"correct_answer\\\":0,\\\"explanation\\\":\\\"...\\\"}]}"
			 " with 10 items.",
			 input_text);

	cJSON_AddStringToObject(root, "model", model);
	cJSON_AddNumberToObject(root, "temperature", 0.6);
	cJSON_AddNumberToObject(root, "max_tokens", 2200);
	cJSON_AddItemToObject(root, "messages", messages);

	cJSON_AddStringToObject(sys, "role", "system");
	cJSON_AddStringToObject(sys, "content", "You are a strict JSON API. Return JSON only.");
	cJSON_AddItemToArray(messages, sys);

	cJSON_AddStringToObject(usr, "role", "user");
	cJSON_AddStringToObject(usr, "content", prompt);
	cJSON_AddItemToArray(messages, usr);

	payload = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return payload;
}

char *openai_generate_quiz_json(const char *input_text, int retries) {
	const char *key = getenv("OPENAI_API_KEY");
	const char *model = getenv("OPENAI_MODEL");
	CURL *curl;
	struct curl_slist *headers = NULL;
	char auth[512];
	char *payload;
	int attempt;

	if (!model || model[0] == '\0') {
		model = "gpt-4o-mini";
	}

	if (!key || key[0] == '\0' || strcmp(key, "replace_with_your_real_openai_key") == 0) {
		log_error("OPENAI_API_KEY not configured, using mock quiz generator");
		return generate_mock_quiz(input_text);
	}

	payload = build_openai_payload(input_text, model);
	if (!payload) {
		return NULL;
	}

	snprintf(auth, sizeof(auth), "Authorization: Bearer %s", key);

	for (attempt = 0; attempt <= retries; attempt++) {
		CURLcode res;
		long status = 0;
		Buffer response = {0};

		curl = curl_easy_init();
		if (!curl) {
			free(payload);
			return NULL;
		}

		headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, auth);

		curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 20000L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res == CURLE_OK && status >= 200 && status < 300) {
			char *result = extract_content_json(response.data ? response.data : "");
			free(response.data);
			free(payload);
			return result;
		}

		log_error("OpenAI call failed attempt=%d status=%ld", attempt + 1, status);
		free(response.data);
	}

	free(payload);
	log_error("All OpenAI attempts failed, falling back to mock quiz");
	return generate_mock_quiz(input_text);
}
