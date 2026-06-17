#include <assert.h>
#include <stdio.h>

#include "db.h"
#include "services.h"

int main(void) {
	const char *quiz = "{\"title\":\"Physics\",\"questions\":["
					   "{\"question\":\"Q1\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q2\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q3\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q4\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q5\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q6\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q7\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q8\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q9\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"},"
					   "{\"question\":\"Q10\",\"options\":[\"a\",\"b\",\"c\",\"d\"],\"correct_answer\":0,\"explanation\":\"e\"}"
					   "]}";

	assert(db_init("./test.db") == 0);
	assert(db_exec_file("./schema.sql") == 0);
	assert(quiz_save("ABC123", quiz) == 0);
	assert(quiz_fetch_by_code("ABC123") != NULL);
	db_close();

	printf("test_db: OK\n");
	return 0;
}
