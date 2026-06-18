"use client";

type LeaderboardEntry = {
	player_name: string;
	score: number;
	xp: number;
};

type LeaderboardProps = {
	items: LeaderboardEntry[];
	currentPlayerName?: string;
};

export function Leaderboard({ items, currentPlayerName }: LeaderboardProps) {
	if (!items || items.length === 0) {
		return (
			<div className="flex h-36 flex-col items-center justify-center rounded-2xl glass p-6 text-center text-text/40">
				<span className="text-3xl">⚔️</span>
				<p className="mt-2 text-sm font-medium">No players in this battle yet.</p>
				<p className="text-xs">Be the first to submit a score!</p>
			</div>
		);
	}

	const getMedalOrRank = (rank: number) => {
		if (rank === 1) return "🥇";
		if (rank === 2) return "🥈";
		if (rank === 3) return "🥉";
		return `#${rank}`;
	};

	const getAvatarBg = (name: string) => {
		const colors = [
			"bg-primary/20 text-primary border-primary/40",
			"bg-secondary/20 text-secondary border-secondary/40",
			"bg-accent/20 text-accent border-accent/40",
			"bg-success/20 text-success border-success/40",
			"bg-amber-500/20 text-amber-300 border-amber-500/40"
		];
		let hash = 0;
		for (let i = 0; i < name.length; i++) {
			hash = name.charCodeAt(i) + ((hash << 5) - hash);
		}
		return colors[Math.abs(hash) % colors.length];
	};

	return (
		<div className="overflow-hidden rounded-2xl p-4 glass shadow-glow-sm">
			<div className="overflow-x-auto">
				<table className="w-full border-collapse text-left text-sm">
					<thead>
						<tr className="border-b border-primary/10 text-[10px] uppercase tracking-wider text-text/45">
							<th className="pb-3 pl-3 font-semibold">Rank</th>
							<th className="pb-3 font-semibold">Player</th>
							<th className="pb-3 text-right font-semibold">XP</th>
							<th className="pb-3 pr-3 text-right font-semibold">Score</th>
						</tr>
					</thead>
					<tbody className="divide-y divide-primary/5">
						{items.map((item, idx) => {
							const rank = idx + 1;
							const isCurrent = currentPlayerName && item.player_name.toLowerCase() === currentPlayerName.toLowerCase();
							const avatarBg = getAvatarBg(item.player_name);
							const avatarLetter = item.player_name.substring(0, 1).toUpperCase();

							return (
								<tr
									key={`${item.player_name}-${idx}`}
									className={`transition-colors hover:bg-primary/5 ${
										isCurrent ? "bg-accent/10 font-bold border-l-4 border-l-accent" : ""
									}`}
								>
									{/* Rank */}
									<td className="py-3 pl-3 font-black text-secondary">
										<span className={rank <= 3 ? "text-xl" : "text-xs"}>
											{getMedalOrRank(rank)}
										</span>
									</td>

									{/* Player Name & Avatar */}
									<td className="py-3">
										<div className="flex items-center gap-3">
											<div
												className={`flex h-8 w-8 items-center justify-center rounded-full border text-xs font-black ${avatarBg}`}
											>
												{avatarLetter}
											</div>
											<span className={isCurrent ? "text-accent" : "text-text/90"}>
												{item.player_name}
												{isCurrent && <span className="ml-1.5 text-[9px] uppercase tracking-wider rounded-md bg-accent/20 px-1 py-0.5 text-accent">You</span>}
											</span>
										</div>
									</td>

									{/* XP */}
									<td className="py-3 text-right font-semibold text-success">
										{item.xp} XP
									</td>

									{/* Score */}
									<td className="py-3 pr-3 text-right font-black text-primary">
										{item.score}
									</td>
								</tr>
							);
						})}
					</tbody>
				</table>
			</div>
		</div>
	);
}
