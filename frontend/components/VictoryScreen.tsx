import Link from "next/link";

export function VictoryScreen({ code }: { code: string }) {
	return (
		<div className="rounded-3xl border border-success/50 bg-success/10 p-8 text-center shadow-glow">
			<h3 className="text-4xl font-black text-success">Victory</h3>
			<p className="mt-2 text-text/85">You finished all 10 questions. Check your final ranking.</p>
			<Link href={`/results/${code}`} className="mt-5 inline-block rounded-xl bg-primary px-5 py-3 font-bold text-bg">
				View Results
			</Link>
		</div>
	);
}
