export function ScoreCounter({ score, streak, xp, level }: { score: number; streak: number; xp: number; level: number }) {
	return (
		<div className="grid grid-cols-4 gap-2 text-center text-sm">
			<Stat label="Score" value={score} />
			<Stat label="Streak" value={streak} />
			<Stat label="XP" value={xp} />
			<Stat label="Level" value={level} />
		</div>
	);
}

function Stat({ label, value }: { label: string; value: number }) {
	return (
		<div className="rounded-xl bg-bg/60 p-2">
			<p className="text-text/70">{label}</p>
			<p className="text-lg font-bold text-secondary">{value}</p>
		</div>
	);
}
