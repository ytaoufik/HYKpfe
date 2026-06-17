export function LoadingScreen({ text }: { text: string }) {
	return (
		<div className="grid min-h-[50vh] place-items-center">
			<div className="animate-pulseSoft rounded-2xl border border-primary/40 bg-card px-8 py-6 text-center shadow-glow">
				<p className="text-lg font-semibold">{text}</p>
			</div>
		</div>
	);
}
