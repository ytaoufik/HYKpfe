#include <assert.h>
#include <stdio.h>

#include "services.h"

void test_openai_run(void) {
	char *result = openai_generate_quiz_json("test", 0);
	assert(result == NULL);
	printf("test_openai: OK (expected failure without API key)\n");
}
