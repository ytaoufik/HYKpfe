export function LivesDisplay({ lives }: { lives: number }) {
	return (
		<div className="flex items-center gap-2 rounded-xl border border-danger/40 bg-danger/10 px-3 py-2">
			{Array.from({ length: 3 }).map((_, i) => (
				<span key={i} className={`text-xl ${i < lives ? "opacity-100" : "opacity-25"}`}>
					❤
				</span>
			))}
		</div>
	);
}
