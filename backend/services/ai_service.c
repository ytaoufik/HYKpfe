#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "services.h"
#include "utils.h"

char *ai_generate_quiz_json(const char *input_text, int retries) {
	const char *provider = getenv("AI_PROVIDER");
	char *result = NULL;

	if (!provider || provider[0] == '\0') {
		provider = "gemini";
	}

	log_info("AI provider: %s", provider);

	if (strcmp(provider, "gemini") == 0) {
		result = gemini_generate_quiz_json(input_text, retries);
		if (!result) {
			log_error("Gemini generation failed, checking OpenAI fallback...");
			const char *openai_key = getenv("OPENAI_API_KEY");
			if (openai_key && openai_key[0] != '\0' && strcmp(openai_key, "replace_with_your_real_openai_key") != 0) {
				log_info("Falling back to OpenAI");
				result = openai_generate_quiz_json(input_text, retries);
			}
		}
	} else {
		result = openai_generate_quiz_json(input_text, retries);
		if (!result) {
			log_error("OpenAI generation failed, checking Gemini fallback...");
			const char *gemini_key = getenv("GEMINI_API_KEY");
			if (gemini_key && gemini_key[0] != '\0' && strcmp(gemini_key, "replace_with_your_real_gemini_key") != 0) {
				log_info("Falling back to Gemini");
				result = gemini_generate_quiz_json(input_text, retries);
			}
		}
	}

	if (!result) {
		log_error("All AI providers failed to generate quiz");
	}

	return result;
}
