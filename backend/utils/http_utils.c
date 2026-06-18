#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *url_encode(const char *src) {
	size_t len = strlen(src);
	char *enc = malloc((len * 3) + 1);
	char *out = enc;
	size_t i;
	if (!enc) {
		return NULL;
	}
	for (i = 0; i < len; i++) {
		unsigned char c = (unsigned char)src[i];
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			*out++ = (char)c;
		} else {
			snprintf(out, 4, "%%%02X", c);
			out += 3;
		}
	}
	*out = '\0';
	return enc;
}

char *url_decode(const char *src) {
	size_t len = strlen(src);
	char *dec = malloc(len + 1);
	char *out = dec;
	size_t i;

	if (!dec) {
		return NULL;
	}

	for (i = 0; i < len; i++) {
		if (src[i] == '%') {
			if (i + 2 < len && isxdigit((unsigned char)src[i+1]) && isxdigit((unsigned char)src[i+2])) {
				char hex[3];
				hex[0] = src[i+1];
				hex[1] = src[i+2];
				hex[2] = '\0';
				*out++ = (char)strtol(hex, NULL, 16);
				i += 2;
			} else {
				*out++ = '%';
			}
		} else if (src[i] == '+') {
			*out++ = ' ';
		} else {
			*out++ = src[i];
		}
	}
	*out = '\0';
	return dec;
}

#include "mongoose.h"
#include "utils.h"

char *dup_mg_string(struct mg_str s) {
	char *out = (char *)malloc((size_t)s.len + 1);
	if (!out) {
		return NULL;
	}
	memcpy(out, s.buf, (size_t)s.len);
	out[s.len] = '\0';
	return out;
}

void respond_json(struct mg_connection *c, int status, const char *json_body) {
	mg_http_reply(c, status,
				  "Content-Type: application/json\r\n"
				  "Access-Control-Allow-Origin: *\r\n"
				  "Access-Control-Allow-Headers: Content-Type\r\n",
				  "%s", json_body ? json_body : "{}");
}
