import Link from "next/link";

export function GameOverScreen({ code }: { code: string }) {
	return (
		<div className="rounded-3xl border border-danger/40 bg-danger/10 p-8 text-center">
			<h3 className="text-4xl font-black">Game Over</h3>
			<p className="mt-2 text-text/85">You ran out of lives. Jump back in and improve your rank.</p>
			<Link href={`/results/${code}`} className="mt-5 inline-block rounded-xl bg-primary px-5 py-3 font-bold text-bg">
				See Results
			</Link>
		</div>
	);
}
