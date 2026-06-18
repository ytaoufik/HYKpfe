#ifndef HYK_APP_H
#define HYK_APP_H

#include "config.h"

typedef struct AppContext {
	AppConfig config;
} AppContext;

int app_load_config(AppContext *ctx);
int app_start_server(AppContext *ctx);
AppContext *app_get_context(void);

#endif
