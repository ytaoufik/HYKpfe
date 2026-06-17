export type QuizQuestion = {
	question: string;
	options: string[];
	correct_answer: number;
	explanation: string;
};

export type QuizPayload = {
	title: string;
	questions: QuizQuestion[];
};

const API_URL = process.env.NEXT_PUBLIC_API_URL || "http://localhost:8080";

async function req<T>(path: string, init?: RequestInit): Promise<T> {
	const res = await fetch(`${API_URL}${path}`, {
		...init,
		headers: {
			"Content-Type": "application/json",
			...(init?.headers || {})
		},
		cache: "no-store"
	});
	const data = await res.json();
	if (!res.ok) {
		throw new Error(data.error || "Request failed");
	}
	return data;
}

export async function generateQuiz(input: string) {
	return req<{ ok: boolean; game_code: string; quiz: QuizPayload }>("/generate-quiz", {
		method: "POST",
		body: JSON.stringify({ input })
	});
}

export async function fetchQuiz(code: string) {
	return req<{ ok: boolean; game_code: string; quiz: QuizPayload }>(`/quiz/${code}`);
}

export async function submitScore(payload: {
	game_code: string;
	player_name: string;
	score: number;
	xp: number;
	streak: number;
	lives: number;
}) {
	return req<{ ok: boolean }>("/submit-score", {
		method: "POST",
		body: JSON.stringify(payload)
	});
}

export async function fetchLeaderboard(code: string) {
	return req<{ ok: boolean; game_code: string; leaderboard: Array<{ player_name: string; score: number; xp: number }> }>(
		`/leaderboard/${code}`
	);
}
