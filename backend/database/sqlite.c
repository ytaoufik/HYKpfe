#include <stddef.h>

#include "db.h"

int sqlite_is_ready(void) {
	return db_handle() != NULL;
}
