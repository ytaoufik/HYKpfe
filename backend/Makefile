CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -I./include -I./third_party
LDFLAGS = -lsqlite3 -lcurl -lm

SRC = \
	main.c \
	api/server.c \
	services/gemini_service.c \
	services/openai_service.c \
	services/ai_service.c \
	services/quiz_service.c \
	services/score_service.c \
	routes/health_routes.c \
	routes/quiz_routes.c \
	routes/score_routes.c \
	routes/session_routes.c \
	database/db.c \
	database/sqlite.c \
	database/supabase.c \
	database/session_db.c \
	database/achievement_db.c \
	models/quiz_model.c \
	models/player_model.c \
	utils/logger.c \
	utils/json_utils.c \
	utils/http_utils.c \
	third_party/mongoose.c \
	third_party/cJSON.c

OBJ = $(SRC:.c=.o)
TARGET = hyk_backend

TEST_SRC = tests/test_runner.c tests/test_quiz.c tests/test_openai.c tests/test_db.c tests/test_integration.c
TEST_TARGET = hyk_tests
TEST_OBJ = $(filter-out main.o, $(OBJ))

all: deps $(TARGET)

deps:
	@if [ ! -f third_party/mongoose.c ] || [ ! -f third_party/mongoose.h ]; then \
		echo "Downloading Mongoose..."; \
		curl -L https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c -o third_party/mongoose.c; \
		curl -L https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h -o third_party/mongoose.h; \
	fi
	@if [ ! -f third_party/cJSON.c ] || [ ! -f third_party/cJSON.h ]; then \
		echo "Downloading cJSON..."; \
		curl -L https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.c -o third_party/cJSON.c; \
		curl -L https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.h -o third_party/cJSON.h; \
	fi

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

test: deps $(OBJ)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(TEST_OBJ) $(LDFLAGS)
	./$(TEST_TARGET)

coverage:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) --coverage" LDFLAGS="$(LDFLAGS) --coverage" test

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJ) tests/*.o *.gcno *.gcda tests/*.gcno tests/*.gcda

.PHONY: all run test clean coverage
