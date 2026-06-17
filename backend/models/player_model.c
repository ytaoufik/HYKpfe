#include <string.h>

int player_model_is_valid_name(const char *name) {
	return name && strlen(name) >= 2 && strlen(name) <= 64;
}
