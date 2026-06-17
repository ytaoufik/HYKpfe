#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HttpBuffer {
	char *data;
	size_t len;
} HttpBuffer;

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	HttpBuffer *mem = (HttpBuffer *)userp;
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

int supabase_ping(const char *supabase_url, const char *supabase_key) {
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	char auth_header[512];
	char bearer_header[512];
	char url[1024];
	long code = 0;
	HttpBuffer response = {0};

	if (!supabase_url || !supabase_key || supabase_url[0] == '\0' || supabase_key[0] == '\0') {
		return -1;
	}

	snprintf(url, sizeof(url), "%s/rest/v1/", supabase_url);
	snprintf(auth_header, sizeof(auth_header), "apikey: %s", supabase_key);
	snprintf(bearer_header, sizeof(bearer_header), "Authorization: Bearer %s", supabase_key);

	curl = curl_easy_init();
	if (!curl) {
		return -1;
	}

	headers = curl_slist_append(headers, auth_header);
	headers = curl_slist_append(headers, bearer_header);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "HYK-Backend/1.0");
	res = curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	free(response.data);

	if (res != CURLE_OK) {
		return -1;
	}
	return (code >= 200 && code < 500) ? 0 : -1;
}
