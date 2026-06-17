export function Timer({ remaining, total }: { remaining: number; total: number }) {
	const pct = Math.max(0, Math.min(100, (remaining / total) * 100));
	return (
		<div className="space-y-2">
			<div className="flex items-center justify-between text-sm">
				<span>Timer</span>
				<span className="font-bold text-secondary">{remaining}s</span>
			</div>
			<div className="h-3 overflow-hidden rounded-full bg-bg/70">
				<div className="h-full rounded-full bg-primary transition-all" style={{ width: `${pct}%` }} />
			</div>
		</div>
	);
}
