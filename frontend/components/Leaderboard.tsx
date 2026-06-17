export function Leaderboard({
	items
}: {
	items: Array<{ player_name: string; score: number; xp: number }>;
}) {
	return (
		<div className="overflow-hidden rounded-2xl border border-primary/30">
			<table className="w-full border-collapse">
				<thead className="bg-primary/20 text-left">
					<tr>
						<th className="p-3">#</th>
						<th className="p-3">Player</th>
						<th className="p-3">Score</th>
						<th className="p-3">XP</th>
					</tr>
				</thead>
				<tbody>
					{items.map((item, i) => (
						<tr key={`${item.player_name}-${i}`} className="border-t border-primary/20">
							<td className="p-3 font-bold text-secondary">{i + 1}</td>
							<td className="p-3">{item.player_name}</td>
							<td className="p-3">{item.score}</td>
							<td className="p-3">{item.xp}</td>
						</tr>
					))}
				</tbody>
			</table>
		</div>
	);
}
