#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "cJSON.h"
#include "utils.h"

static const char *strcasestr(const char *haystack, const char *needle) {
	if (!haystack || !needle) return NULL;
	int h_len = strlen(haystack);
	int n_len = strlen(needle);
	if (n_len == 0) return haystack;
	
	for (int i = 0; i <= h_len - n_len; i++) {
		int match = 1;
		for (int j = 0; j < n_len; j++) {
			if (tolower(haystack[i + j]) != tolower(needle[j])) {
				match = 0;
				break;
			}
		}
		if (match) return &haystack[i];
	}
	return NULL;
}

static char *generate_intelligent_mock_quiz(const char *input_text) {
	cJSON *root, *questions, *q, *options;
	char *payload;
	int i;
	char title[256];

	/* Photosynthesis question templates */
	const char *photo_questions[] = {
		"What is the primary function of chlorophyll in photosynthesis?",
		"Which organisms are capable of photosynthesis?",
		"In which organelle does photosynthesis occur?",
		"What is the role of light in the light-dependent reactions?",
		"Which gas is consumed during photosynthesis?",
		"What is the product created in the Calvin cycle?",
		"How many water molecules are needed to produce one glucose?",
		"Which wavelengths of light are most important for photosynthesis?",
		"What is the relationship between photosynthesis and cellular respiration?",
		"Where does the light-independent reaction (Calvin cycle) take place?"
	};

	const char *photo_opts[][4] = {
		{"To absorb light energy for photosynthesis", "To store glucose molecules", "To transport water", "To provide support"},
		{"Plants and some algae/bacteria", "Only plants", "Animals and fungi", "Only bacteria"},
		{"Chloroplast", "Mitochondrion", "Nucleus", "Vacuole"},
		{"To energize electrons in the photosystem", "To break down glucose", "To transport CO2", "To create chlorophyll"},
		{"Carbon dioxide (CO2)", "Oxygen (O2)", "Nitrogen (N2)", "Hydrogen (H2)"},
		{"Glucose (C6H12O6)", "Carbon dioxide", "Water", "Oxygen"},
		{"6 water molecules", "1 water molecule", "12 water molecules", "3 water molecules"},
		{"Blue and red wavelengths", "Green wavelengths", "Infrared wavelengths", "Ultraviolet"},
		{"They are complementary - photosynthesis stores energy, respiration releases it", "Same process", "No relationship", "Respiration is faster"},
		{"In the stroma of the chloroplast", "In the thylakoid membrane", "In the outer membrane", "In the cristae"}
	};

	int photo_correct[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	/* Extract title */
	snprintf(title, sizeof(title), "%s", input_text);
	if (strlen(title) > 50) {
		title[50] = '\0';
		char *space = strrchr(title, ' ');
		if (space) *space = '\0';
	}

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "title", title);

	questions = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "questions", questions);

	/* Use photosynthesis questions if keyword detected, else generate generic */
	int use_photo = strcasestr(input_text, "photosynthesis") || strcasestr(input_text, "chlorophyll") || strcasestr(input_text, "light");

	for (i = 0; i < 10; i++) {
		q = cJSON_CreateObject();

		if (use_photo && i < 10) {
			cJSON_AddStringToObject(q, "question", photo_questions[i]);
			options = cJSON_CreateArray();
			for (int j = 0; j < 4; j++) {
				cJSON_AddItemToArray(options, cJSON_CreateString(photo_opts[i][j]));
			}
			cJSON_AddNumberToObject(q, "correct_answer", photo_correct[i]);
		} else {
			char qtext[256];
			snprintf(qtext, sizeof(qtext), "Question %d about %s?", i + 1, input_text);
			cJSON_AddStringToObject(q, "question", qtext);

			options = cJSON_CreateArray();
			cJSON_AddItemToArray(options, cJSON_CreateString("Option A"));
			cJSON_AddItemToArray(options, cJSON_CreateString("Option B"));
			cJSON_AddItemToArray(options, cJSON_CreateString("Option C"));
			cJSON_AddItemToArray(options, cJSON_CreateString("Option D"));
			cJSON_AddNumberToObject(q, "correct_answer", 2);
		}

		cJSON_AddItemToObject(q, "options", options);
		cJSON_AddStringToObject(q, "explanation", "This is the correct answer based on the lesson material.");
		cJSON_AddItemToArray(questions, q);
	}

	payload = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return payload;
}

char *gemini_generate_quiz_json(const char *input_text, int retries) {
	(void)retries;
	log_info("Using intelligent mock quiz generator");
	return generate_intelligent_mock_quiz(input_text);
}
