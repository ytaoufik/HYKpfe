"use client";

import { useEffect, useState, useRef } from "react";

type ScoreCounterProps = {
	score: number;
	streak: number;
	xp: number;
	level: number;
};

export function ScoreCounter({ score, streak, xp, level }: ScoreCounterProps) {
	const [pulseScore, setPulseScore] = useState(false);
	const [pulseStreak, setPulseStreak] = useState(false);
	const [pulseLevel, setPulseLevel] = useState(false);

	const prevScore = useRef(score);
	const prevStreak = useRef(streak);
	const prevLevel = useRef(level);

	useEffect(() => {
		if (score !== prevScore.current) {
			setPulseScore(true);
			const t = setTimeout(() => setPulseScore(false), 300);
			prevScore.current = score;
			return () => clearTimeout(t);
		}
	}, [score]);

	useEffect(() => {
		if (streak !== prevStreak.current) {
			setPulseStreak(true);
			const t = setTimeout(() => setPulseStreak(false), 300);
			prevStreak.current = streak;
			return () => clearTimeout(t);
		}
	}, [streak]);

	useEffect(() => {
		if (level !== prevLevel.current) {
			setPulseLevel(true);
			const t = setTimeout(() => setPulseLevel(false), 300);
			prevLevel.current = level;
			return () => clearTimeout(t);
		}
	}, [level]);

	// XP progress to next level (500 XP per level)
	const xpInLevel = xp % 500;
	const xpPercent = (xpInLevel / 500) * 100;

	return (
		<div className="grid grid-cols-2 gap-3 sm:grid-cols-4">
			{/* Score */}
			<div className="flex flex-col items-center justify-center rounded-2xl p-3 glass shadow-glow-sm transition-all duration-300">
				<span className="text-xl">🏆</span>
				<span className="mt-1 text-[10px] uppercase tracking-wider text-text/50">Score</span>
				<span
					className={`mt-1 text-2xl font-black text-primary transition-transform duration-200 ${
						pulseScore ? "scale-125 text-secondary animate-pulseSoft" : ""
					}`}
				>
					{score}
				</span>
			</div>

			{/* Streak */}
			<div
				className={`flex flex-col items-center justify-center rounded-2xl p-3 glass shadow-glow-sm transition-all duration-300 ${
					streak >= 3 ? "border-amber-500/40 bg-amber-500/5" : ""
				}`}
			>
				<span className="text-xl">{streak >= 3 ? "🔥" : "✨"}</span>
				<span className="mt-1 text-[10px] uppercase tracking-wider text-text/50">Streak</span>
				<span
					className={`mt-1 text-2xl font-black transition-all duration-200 ${
						streak >= 3 ? "text-amber-400 drop-shadow-[0_0_10px_rgba(245,158,11,0.6)]" : "text-text/80"
					} ${pulseStreak ? "scale-125" : ""}`}
				>
					{streak}
					{streak >= 3 && <span className="ml-1 text-xs animate-bounce">🔥</span>}
				</span>
			</div>

			{/* XP Progress */}
			<div className="flex flex-col justify-center rounded-2xl p-3 glass shadow-glow-sm">
				<div className="flex items-center justify-between text-[10px] uppercase tracking-wider text-text/50">
					<span>XP Progress</span>
					<span className="font-bold text-success">{xpInLevel}/500</span>
				</div>
				<div className="mt-2 h-2.5 w-full overflow-hidden rounded-full bg-bg/85">
					<div
						className="h-full rounded-full bg-gradient-to-r from-success to-accent transition-all duration-500 ease-out shadow-[0_0_8px_rgba(167,243,208,0.4)]"
						style={{ width: `${xpPercent}%` }}
					/>
				</div>
				<span className="mt-1.5 text-center text-xs font-semibold text-text/80">
					{xp} Total XP
				</span>
			</div>

			{/* Level */}
			<div className="flex flex-col items-center justify-center rounded-2xl p-3 glass shadow-glow-sm">
				<span className="text-xl">⭐</span>
				<span className="mt-1 text-[10px] uppercase tracking-wider text-text/50">Level</span>
				<span
					className={`mt-1 inline-flex items-center justify-center rounded-full bg-gradient-to-r from-accent to-secondary px-3.5 py-0.5 text-lg font-black text-bg shadow-[0_0_12px_rgba(210,124,168,0.5)] transition-transform duration-300 ${
						pulseLevel ? "scale-125 animate-levelUp" : ""
					}`}
				>
					Lv. {level}
				</span>
			</div>
		</div>
	);
}
