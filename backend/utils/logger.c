#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

static void log_with_level(const char *level, const char *fmt, va_list args) {
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);
	char ts[32];

	if (tm_info) {
		strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);
	} else {
		snprintf(ts, sizeof(ts), "unknown-time");
	}

	fprintf(stdout, "[%s] [%s] ", ts, level);
	vfprintf(stdout, fmt, args);
	fputc('\n', stdout);
}

void log_info(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_with_level("INFO", fmt, args);
	va_end(args);
}

void log_error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_with_level("ERROR", fmt, args);
	va_end(args);
}

char *read_file_text(const char *path) {
	FILE *f = fopen(path, "rb");
	long size;
	char *buffer;

	if (!f) {
		return NULL;
	}

	if (fseek(f, 0, SEEK_END) != 0) {
		fclose(f);
		return NULL;
	}

	size = ftell(f);
	if (size < 0) {
		fclose(f);
		return NULL;
	}
	rewind(f);

	buffer = (char *)malloc((size_t)size + 1);
	if (!buffer) {
		fclose(f);
		return NULL;
	}

	if (fread(buffer, 1, (size_t)size, f) != (size_t)size) {
		free(buffer);
		fclose(f);
		return NULL;
	}

	buffer[size] = '\0';
	fclose(f);
	return buffer;
}
