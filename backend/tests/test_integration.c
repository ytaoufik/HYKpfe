#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define setenv(name,val,overwrite) _putenv_s(name,val)
#endif

#include "services.h"

void test_integration_run(void) {
	/* Ensure fallback happens gracefully */
	setenv("AI_PROVIDER", "gemini", 1);
	setenv("GEMINI_API_KEY", "", 1);
	setenv("OPENAI_API_KEY", "", 1);

	char *res = ai_generate_quiz_json("integration test content", 0);
	assert(res == NULL); /* Should fail safely without keys */
	printf("test_integration: OK\n");
}
