"use client";

import { useEffect, useState } from "react";
import { useParams } from "next/navigation";
import { fetchLeaderboard } from "../../../lib/api";
import { Leaderboard } from "../../../components/Leaderboard";

export default function LeaderboardPage() {
	const params = useParams<{ code: string }>();
	const code = (params?.code || "").toUpperCase();
	const [items, setItems] = useState<Array<{ player_name: string; score: number; xp: number }>>([]);

	useEffect(() => {
		if (!code) return;
		fetchLeaderboard(code).then((data) => setItems(data.leaderboard)).catch(() => setItems([]));
	}, [code]);

	return (
		<section className="space-y-6">
			<h2 className="text-3xl font-extrabold">Leaderboard - {code}</h2>
			<Leaderboard items={items} />
		</section>
	);
}
