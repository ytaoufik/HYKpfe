"use client";

import { useRouter } from "next/navigation";
import { useState } from "react";

export default function JoinPage() {
	const router = useRouter();
	const [code, setCode] = useState("");
	const [name, setName] = useState("");

	function onJoin() {
		const gameCode = code.trim().toUpperCase();
		const playerName = name.trim();
		if (!gameCode || !playerName) return;

		localStorage.setItem("hyk_player_name", playerName);
		router.push(`/play/${gameCode}?player=${encodeURIComponent(playerName)}`);
	}

	return (
		<section className="mx-auto max-w-xl space-y-6 rounded-3xl border border-primary/20 bg-card/80 p-8">
			<h2 className="text-3xl font-extrabold">Join a Battle</h2>
			<input
				className="w-full rounded-xl border border-primary/30 bg-bg/60 p-3"
				placeholder="Game code"
				value={code}
				onChange={(e) => setCode(e.target.value)}
			/>
			<input
				className="w-full rounded-xl border border-primary/30 bg-bg/60 p-3"
				placeholder="Your nickname"
				value={name}
				onChange={(e) => setName(e.target.value)}
			/>
			<button className="w-full rounded-xl bg-primary px-5 py-3 font-bold text-bg" onClick={onJoin}>
				Start Playing
			</button>
		</section>
	);
}
