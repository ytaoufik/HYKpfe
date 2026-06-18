#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "db.h"
#include "services.h"

void test_db_run(void) {
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

	/* Clean up old test data */
	sqlite3_exec(db_handle(), "DELETE FROM quizzes;", NULL, NULL, NULL);
	sqlite3_exec(db_handle(), "DELETE FROM sessions;", NULL, NULL, NULL);
	sqlite3_exec(db_handle(), "DELETE FROM achievements;", NULL, NULL, NULL);

	/* Test Quiz */
	assert(quiz_save("ABC123", quiz) == 0);
	char *fetched_quiz = quiz_fetch_by_code("ABC123");
	assert(fetched_quiz != NULL);
	free(fetched_quiz);

	/* Test Sessions */
	assert(session_save("ABC123", "yasmine", "{\"score\":100}") == 0);
	char *fetched_session = session_fetch("ABC123", "yasmine");
	assert(fetched_session != NULL);
	assert(strstr(fetched_session, "score") != NULL);
	free(fetched_session);

	/* Test Achievements */
	assert(achievement_save("ABC123", "yasmine", "first_blood") == 0);
	assert(achievement_save("ABC123", "yasmine", "streak_3") == 0);
	char *ach_json = achievements_fetch("ABC123", "yasmine");
	assert(ach_json != NULL);
	assert(strstr(ach_json, "first_blood") != NULL);
	assert(strstr(ach_json, "streak_3") != NULL);
	free(ach_json);

	db_close();
	printf("test_db: OK\n");
}
