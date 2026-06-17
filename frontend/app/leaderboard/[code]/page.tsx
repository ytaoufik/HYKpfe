"use client";

import { useEffect, useState } from "react";
import { fetchLeaderboard } from "../../../lib/api";
import { Leaderboard } from "../../../components/Leaderboard";

export default function LeaderboardPage({ params }: { params: { code: string } }) {
	const [items, setItems] = useState<Array<{ player_name: string; score: number; xp: number }>>([]);

	useEffect(() => {
		fetchLeaderboard(params.code.toUpperCase()).then((data) => setItems(data.leaderboard)).catch(() => setItems([]));
	}, [params.code]);

	return (
		<section className="space-y-6">
			<h2 className="text-3xl font-extrabold">Leaderboard - {params.code.toUpperCase()}</h2>
			<Leaderboard items={items} />
		</section>
	);
}
