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
