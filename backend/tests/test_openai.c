#include <assert.h>
#include <stdio.h>

#include "services.h"

int main(void) {
	char *result = openai_generate_quiz_json("test", 0);
	assert(result == NULL);
	printf("test_openai: OK (expected failure without API key)\n");
	return 0;
}
