export const BASE_SCORE = 100;

export function calculateScore(remainingSeconds: number, streak: number, isCorrect: boolean): number {
	if (!isCorrect) return 0;
	const speedBonus = remainingSeconds * 2;
	const streakBonus = streak > 0 && streak % 3 === 0 ? 200 : 0;
	return BASE_SCORE + speedBonus + streakBonus;
}

export function calculateXp(scoreDelta: number, isCorrect: boolean): number {
	return isCorrect ? Math.max(20, Math.floor(scoreDelta / 5)) : 5;
}

export function calculateLevel(xp: number): number {
	return Math.floor(xp / 500) + 1;
}

export function isPerfectQuiz(correctCount: number, total: number): boolean {
	return total > 0 && correctCount === total;
}

export function bonusPerfectQuiz(): number {
	return 1000;
}

export type Achievement = { key: string; title: string; description: string; icon: string };

const ACHIEVEMENT_DEFS: Achievement[] = [
	{ key: "first_blood", title: "First Blood", description: "First correct answer", icon: "🩸" },
	{ key: "speed_demon", title: "Speed Demon", description: "Answered in under 5 seconds", icon: "⚡" },
	{ key: "comeback_king", title: "Comeback King", description: "Won with only 1 life remaining", icon: "👑" },
	{ key: "streak_3", title: "Hot Streak", description: "3 correct answers in a row", icon: "🔥" },
	{ key: "streak_5", title: "On Fire", description: "5 correct answers in a row", icon: "💥" },
	{ key: "streak_10", title: "Unstoppable", description: "10 correct in a row — perfect!", icon: "🌟" },
	{ key: "survivor", title: "Survivor", description: "Never lost a life", icon: "🛡️" },
	{ key: "perfect", title: "Flawless Mind", description: "Perfect quiz completed", icon: "💎" },
	{ key: "level_3", title: "Rising Star", description: "Reached level 3", icon: "⭐" }
];

export interface AchievementCheckArgs {
	streak: number;
	level: number;
	perfect: boolean;
	correctCount: number;
	lives: number;
	totalLives: number;
	totalQuestions: number;
	answerTimeSeconds: number;
}

export function resolveAchievements(args: AchievementCheckArgs): Achievement[] {
	const list: Achievement[] = [];

	if (args.correctCount === 1) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "first_blood");
		if (a) list.push(a);
	}
	if (args.answerTimeSeconds < 5 && args.correctCount > 0) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "speed_demon");
		if (a) list.push(a);
	}
	if (args.lives === 1 && args.perfect === false && args.correctCount === args.totalQuestions) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "comeback_king");
		if (a) list.push(a);
	}
	if (args.streak >= 3 && args.streak < 5) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "streak_3");
		if (a) list.push(a);
	}
	if (args.streak >= 5 && args.streak < 10) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "streak_5");
		if (a) list.push(a);
	}
	if (args.streak >= 10) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "streak_10");
		if (a) list.push(a);
	}
	if (args.lives === args.totalLives && args.correctCount === args.totalQuestions) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "survivor");
		if (a) list.push(a);
	}
	if (args.perfect) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "perfect");
		if (a) list.push(a);
	}
	if (args.level >= 3) {
		const a = ACHIEVEMENT_DEFS.find((d) => d.key === "level_3");
		if (a) list.push(a);
	}

	return list;
}

export type Rank = "Master" | "Diamond" | "Gold" | "Silver" | "Bronze";

export function calculateRank(score: number, correctCount: number, total: number): Rank {
	if (total <= 0) return "Bronze";
	const pct = correctCount / total;
	if (pct >= 1.0) return "Master";
	if (pct >= 0.9) return "Diamond";
	if (pct >= 0.7) return "Gold";
	if (pct >= 0.5) return "Silver";
	return "Bronze";
}

export type Grade = "A+" | "A" | "B+" | "B" | "C" | "D" | "F";

export function calculateGrade(correctCount: number, total: number): Grade {
	if (total <= 0) return "F";
	const pct = correctCount / total;
	if (pct >= 1.0) return "A+";
	if (pct >= 0.9) return "A";
	if (pct >= 0.8) return "B+";
	if (pct >= 0.7) return "B";
	if (pct >= 0.5) return "C";
	if (pct >= 0.3) return "D";
	return "F";
}

export function gradeColor(grade: Grade): string {
	switch (grade) {
		case "A+":
		case "A":
			return "text-success";
		case "B+":
		case "B":
			return "text-primary";
		case "C":
			return "text-warning";
		case "D":
		case "F":
			return "text-danger";
	}
}

export function rankColor(rank: Rank): string {
	switch (rank) {
		case "Master":
			return "text-success";
		case "Diamond":
			return "text-secondary";
		case "Gold":
			return "text-warning";
		case "Silver":
			return "text-text/70";
		case "Bronze":
			return "text-accent";
	}
}
