#include <stdio.h>

void test_quiz_run(void);
void test_db_run(void);
void test_openai_run(void);
void test_integration_run(void);

int main(void) {
	printf("=== Running Backend Test Suite ===\n");
	test_quiz_run();
	test_db_run();
	test_openai_run();
	test_integration_run();
	printf("All backend tests completed successfully!\n");
	return 0;
}
