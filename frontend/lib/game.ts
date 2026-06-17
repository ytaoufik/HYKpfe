export const BASE_SCORE = 100;

export function calculateScore(remainingSeconds: number, streak: number, isCorrect: boolean) {
	if (!isCorrect) return 0;
	const speedBonus = remainingSeconds * 2;
	const streakBonus = streak > 0 && streak % 3 === 0 ? 200 : 0;
	return BASE_SCORE + speedBonus + streakBonus;
}

export function calculateXp(scoreDelta: number, isCorrect: boolean) {
	return isCorrect ? Math.max(20, Math.floor(scoreDelta / 5)) : 5;
}

export function calculateLevel(xp: number) {
	return Math.floor(xp / 500) + 1;
}

export function isPerfectQuiz(correctCount: number, total: number) {
	return total > 0 && correctCount === total;
}

export function bonusPerfectQuiz() {
	return 1000;
}

export type Achievement = { key: string; title: string; description: string };

export function resolveAchievements(args: { streak: number; level: number; perfect: boolean }): Achievement[] {
	const list: Achievement[] = [];
	if (args.streak >= 3) {
		list.push({ key: "streak_3", title: "Hot Streak", description: "3 correct answers in a row" });
	}
	if (args.level >= 3) {
		list.push({ key: "level_3", title: "Rising Star", description: "Reached level 3" });
	}
	if (args.perfect) {
		list.push({ key: "perfect", title: "Flawless Mind", description: "Perfect quiz completed" });
	}
	return list;
}
