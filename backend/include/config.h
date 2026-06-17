#ifndef HYK_CONFIG_H
#define HYK_CONFIG_H

#define HYK_MAX_JSON_BODY 65536
#define HYK_GAME_CODE_LEN 7
#define HYK_MAX_PLAYERS 512

typedef struct AppConfig {
	const char *port;
	const char *sqlite_path;
	const char *openai_api_key;
	const char *openai_model;
	const char *supabase_url;
	const char *supabase_key;
	int request_timeout_ms;
	int openai_max_retries;
} AppConfig;

#endif
