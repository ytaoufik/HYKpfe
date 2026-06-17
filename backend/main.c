#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "app.h"
#include "db.h"
#include "utils.h"

static const char *env_or_default(const char *key, const char *fallback) {
	const char *v = getenv(key);
	return (v && v[0] != '\0') ? v : fallback;
}

static void trim_in_place(char *s) {
	char *start = s;
	char *end;

	while (*start && isspace((unsigned char)*start)) {
		start++;
	}
	if (start != s) {
		memmove(s, start, strlen(start) + 1);
	}

	end = s + strlen(s);
	while (end > s && isspace((unsigned char)*(end - 1))) {
		end--;
	}
	*end = '\0';
}

static void unquote_in_place(char *s) {
	size_t len = strlen(s);
	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"') || (s[0] == '\'' && s[len - 1] == '\''))) {
		memmove(s, s + 1, len - 2);
		s[len - 2] = '\0';
	}
}

static void load_dotenv_file(const char *path) {
	FILE *f = fopen(path, "r");
	char line[4096];

	if (!f) {
		return;
	}

	while (fgets(line, sizeof(line), f)) {
		char *eq;
		char *key;
		char *value;

		trim_in_place(line);
		if (line[0] == '\0' || line[0] == '#') {
			continue;
		}

		eq = strchr(line, '=');
		if (!eq) {
			continue;
		}

		*eq = '\0';
		key = line;
		value = eq + 1;

		trim_in_place(key);
		trim_in_place(value);
		unquote_in_place(value);

		if (key[0] != '\0' && getenv(key) == NULL) {
#if defined(_WIN32)
			{
				char envline[4096];
				snprintf(envline, sizeof(envline), "%s=%s", key, value);
				_putenv(envline);
			}
#else
			setenv(key, value, 0);
#endif
		}
	}

	fclose(f);
}

int app_load_config(AppContext *ctx) {
	if (!ctx) {
		return -1;
	}

	ctx->config.port = env_or_default("PORT", "8080");
	ctx->config.sqlite_path = env_or_default("SQLITE_DB_PATH", "./hyk.db");
	ctx->config.openai_api_key = env_or_default("OPENAI_API_KEY", "");
	ctx->config.openai_model = env_or_default("OPENAI_MODEL", "gpt-4o-mini");
	ctx->config.supabase_url = env_or_default("SUPABASE_URL", "");
	ctx->config.supabase_key = env_or_default("SUPABASE_KEY", "");
	ctx->config.request_timeout_ms = atoi(env_or_default("HTTP_TIMEOUT_MS", "20000"));
	ctx->config.openai_max_retries = atoi(env_or_default("OPENAI_MAX_RETRIES", "2"));

	if (ctx->config.request_timeout_ms <= 0) {
		ctx->config.request_timeout_ms = 20000;
	}
	if (ctx->config.openai_max_retries < 0) {
		ctx->config.openai_max_retries = 2;
	}
	return 0;
}

int main(void) {
	AppContext ctx;
	memset(&ctx, 0, sizeof(ctx));

	load_dotenv_file("./.env");
	load_dotenv_file("../.env");

	if (app_load_config(&ctx) != 0) {
		fprintf(stderr, "Failed to load configuration\n");
		return 1;
	}

	if (db_init(ctx.config.sqlite_path) != 0) {
		fprintf(stderr, "Failed to initialize SQLite database\n");
		return 1;
	}

	if (db_run_migrations() != 0) {
		fprintf(stderr, "Failed to run migrations\n");
		db_close();
		return 1;
	}

	log_info("HYK backend started on port %s", ctx.config.port);
	int rc = app_start_server(&ctx);
	db_close();
	return rc;
}
