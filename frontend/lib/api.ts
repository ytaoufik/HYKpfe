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

export type LeaderboardEntry = {
	player_name: string;
	score: number;
	xp: number;
};

const API_URL = process.env.NEXT_PUBLIC_API_URL || "http://localhost:8080";
const TIMEOUT_MS = 15_000;
const MAX_RETRIES = 3;
const RETRY_DELAY_MS = 1_000;

function withTimeout(ms: number): AbortSignal {
	const controller = new AbortController();
	setTimeout(() => controller.abort(), ms);
	return controller.signal;
}

async function sleep(ms: number): Promise<void> {
	return new Promise((resolve) => setTimeout(resolve, ms));
}

async function req<T>(path: string, init?: RequestInit, retries = MAX_RETRIES): Promise<T> {
	let lastError: Error | null = null;

	for (let attempt = 0; attempt < retries; attempt++) {
		try {
			const res = await fetch(`${API_URL}${path}`, {
				...init,
				headers: {
					"Content-Type": "application/json",
					...(init?.headers || {})
				},
				cache: "no-store",
				signal: withTimeout(TIMEOUT_MS)
			});

			const data = await res.json();

			if (!res.ok) {
				throw new Error(data.error || `Request failed with status ${res.status}`);
			}
			return data;
		} catch (err) {
			lastError = err instanceof Error ? err : new Error(String(err));
			if (attempt < retries - 1) {
				await sleep(RETRY_DELAY_MS * (attempt + 1));
			}
		}
	}

	throw lastError || new Error("Request failed after retries");
}

export async function generateQuiz(input: string, inputType?: string) {
	return req<{ ok: boolean; game_code: string; quiz: QuizPayload }>("/generate-quiz", {
		method: "POST",
		body: JSON.stringify({ input, input_type: inputType || "subject" })
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
	return req<{ ok: boolean; game_code: string; leaderboard: LeaderboardEntry[] }>(
		`/leaderboard/${code}`
	);
}

export async function saveSessionAPI(code: string, playerName: string, stateJson: string) {
	return req<{ ok: boolean }>("/session/save", {
		method: "POST",
		body: JSON.stringify({ game_code: code, player_name: playerName, state: stateJson })
	});
}

export async function fetchSessionAPI(code: string, playerName: string) {
	return req<{ ok: boolean; state: string }>(`/session/${code}/${encodeURIComponent(playerName)}`);
}

export async function fetchStats() {
	return req<{ ok: boolean; total_games: number; total_players: number; total_questions: number }>("/stats");
}
