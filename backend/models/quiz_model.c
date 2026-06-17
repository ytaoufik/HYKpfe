#include <string.h>

int quiz_model_is_valid_code(const char *code) {
	return code && strlen(code) == 6;
}
