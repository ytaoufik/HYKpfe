type Achievement = { key: string; title: string; description: string };

export function AchievementPopup({ achievement }: { achievement: Achievement | null }) {
	if (!achievement) return null;
	return (
		<div className="fixed bottom-6 right-6 animate-floatIn rounded-xl border border-success/50 bg-success/15 p-4 shadow-glow">
			<p className="text-xs uppercase tracking-widest text-success">Achievement Unlocked</p>
			<p className="text-lg font-black">{achievement.title}</p>
			<p className="text-sm text-text/85">{achievement.description}</p>
		</div>
	);
}
