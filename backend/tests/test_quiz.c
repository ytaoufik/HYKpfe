#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "services.h"

static const char *valid_quiz =
	"{\"title\":\"Math\",\"questions\":["
	"{\"question\":\"Q1\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q2\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":1,\"explanation\":\"e\"},"
	"{\"question\":\"Q3\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":2,\"explanation\":\"e\"},"
	"{\"question\":\"Q4\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":3,\"explanation\":\"e\"},"
	"{\"question\":\"Q5\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q6\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q7\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q8\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q9\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
	"{\"question\":\"Q10\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"}"
	"]}";

int main(void) {
	char error[128] = {0};
	int ok = quiz_validate_payload(valid_quiz, error, sizeof(error));
	assert(ok == 1);
	assert(strlen(error) == 0);

	ok = quiz_validate_payload("{\"title\":\"Broken\",\"questions\":[]}", error, sizeof(error));
	assert(ok == 0);

	printf("test_quiz: OK\n");
	return 0;
}
