import Link from "next/link";

export default function HomePage() {
	return (
		<section className="animate-floatIn space-y-8">
			<div className="rounded-3xl border border-primary/30 bg-card/80 p-10 shadow-glow">
				<p className="text-sm uppercase tracking-[0.35em] text-secondary">H.Y.K - How You Know</p>
				<h1 className="mt-4 text-5xl font-black leading-tight text-text">AI Quiz Battle</h1>
				<p className="mt-5 max-w-2xl text-lg text-text/85">
					Generate playable 10-question quizzes from any lesson, battle in real time, and climb the leaderboard with lives,
					streaks, XP, and achievements.
				</p>
				<div className="mt-8 flex flex-wrap gap-4">
					<Link href="/teacher" className="rounded-xl bg-primary px-6 py-3 font-bold text-bg transition hover:bg-secondary">
						Teacher Mode
					</Link>
					<Link href="/join" className="rounded-xl border border-primary px-6 py-3 font-bold text-primary transition hover:bg-primary/10">
						Join Game
					</Link>
				</div>
			</div>
		</section>
	);
}
