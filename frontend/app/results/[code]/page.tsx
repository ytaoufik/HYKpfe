"use client";

import Link from "next/link";
import { useMemo } from "react";

export default function ResultsPage({ params }: { params: { code: string } }) {
	const summary = useMemo(() => {
		if (typeof window === "undefined") return null;
		const raw = localStorage.getItem(`hyk_session_${params.code.toUpperCase()}`);
		return raw ? JSON.parse(raw) : null;
	}, [params.code]);

	return (
		<section className="space-y-6 rounded-3xl border border-primary/30 bg-card/90 p-8">
			<h2 className="text-3xl font-extrabold">Battle Results</h2>
			<div className="grid grid-cols-2 gap-4 md:grid-cols-4">
				<Stat label="Score" value={summary?.score ?? 0} />
				<Stat label="XP" value={summary?.xp ?? 0} />
				<Stat label="Level" value={summary?.level ?? 1} />
				<Stat label="Correct" value={summary?.correctCount ?? 0} />
			</div>
			<div className="flex flex-wrap gap-3">
				<Link className="rounded-xl bg-primary px-4 py-2 font-bold text-bg" href={`/leaderboard/${params.code.toUpperCase()}`}>
					View Leaderboard
				</Link>
				<Link className="rounded-xl border border-primary px-4 py-2 font-bold text-primary" href="/join">
					Play Again
				</Link>
			</div>
		</section>
	);
}

function Stat({ label, value }: { label: string; value: number }) {
	return (
		<div className="rounded-xl bg-bg/70 p-4">
			<p className="text-sm text-text/70">{label}</p>
			<p className="text-2xl font-black">{value}</p>
		</div>
	);
}
