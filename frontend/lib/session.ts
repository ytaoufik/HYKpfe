import { saveSessionAPI, fetchSessionAPI } from "./api";

const STORAGE_PREFIX = "hyk_session_";

function storageKey(code: string, playerName: string): string {
	return `${STORAGE_PREFIX}${code}_${playerName}`;
}

export interface GameSessionState {
	currentQuestion: number;
	score: number;
	xp: number;
	streak: number;
	lives: number;
	correctCount: number;
	answers: Array<{ questionIndex: number; selectedOption: number; isCorrect: boolean; timeRemaining: number }>;
	achievements: string[];
	level: number;
	bestStreak: number;
}

export function saveSession(code: string, playerName: string, state: GameSessionState): void {
	try {
		const json = JSON.stringify(state);
		if (typeof window !== "undefined" && window.localStorage) {
			localStorage.setItem(storageKey(code, playerName), json);
		}
		saveSessionAPI(code, playerName, json).catch(() => {
			/* silent — localStorage is primary */
		});
	} catch {
		/* ignore write errors */
	}
}

export async function restoreSession(code: string, playerName: string): Promise<GameSessionState | null> {
	try {
		const apiResult = await fetchSessionAPI(code, playerName);
		if (apiResult.ok && apiResult.state) {
			const parsed: GameSessionState = JSON.parse(apiResult.state);
			return parsed;
		}
	} catch {
		/* fall through to localStorage */
	}

	try {
		if (typeof window !== "undefined" && window.localStorage) {
			const raw = localStorage.getItem(storageKey(code, playerName));
			if (raw) {
				return JSON.parse(raw) as GameSessionState;
			}
		}
	} catch {
		/* ignore */
	}

	return null;
}

export function clearSession(code: string, playerName?: string): void {
	try {
		if (typeof window !== "undefined" && window.localStorage) {
			if (playerName) {
				localStorage.removeItem(storageKey(code, playerName));
			} else {
				const keys = Object.keys(localStorage);
				for (const key of keys) {
					if (key.startsWith(`${STORAGE_PREFIX}${code}_`)) {
						localStorage.removeItem(key);
					}
				}
			}
		}
	} catch {
		/* ignore */
	}
}
