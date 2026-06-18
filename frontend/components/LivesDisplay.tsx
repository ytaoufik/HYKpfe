"use client";

type LivesDisplayProps = {
	lives: number;
	maxLives?: number;
};

export function LivesDisplay({ lives, maxLives = 3 }: LivesDisplayProps) {
	return (
		<div className="flex items-center gap-1.5 rounded-2xl px-4 py-2.5 glass shadow-glow-sm">
			<span className="mr-1 text-xs font-semibold uppercase tracking-wider text-text/50">
				Lives
			</span>
			<div className="flex items-center gap-1">
				{Array.from({ length: maxLives }).map((_, i) => {
					const isFilled = i < lives;
					const isJustBroken = i === lives; // The one that was just lost

					return (
						<svg
							key={i}
							xmlns="http://www.w3.org/2000/svg"
							viewBox="0 0 24 24"
							className={`h-6 w-6 transition-all duration-500 ${
								isFilled
									? "fill-danger stroke-danger drop-shadow-[0_0_8px_rgba(251,113,133,0.6)] animate-heartbeat"
									: isJustBroken
									? "fill-danger/10 stroke-danger/30 scale-75 rotate-12 opacity-30 animate-breakHeart"
									: "fill-danger/5 stroke-danger/20 scale-90 opacity-20"
							}`}
							style={{
								animationDelay: `${i * 0.1}s`,
								transformOrigin: "center"
							}}
						>
							<path
								strokeLinecap="round"
								strokeLinejoin="round"
								strokeWidth="2"
								d="M4.318 6.318a4.5 4.5 0 000 6.364L12 20.364l7.682-7.682a4.5 4.5 0 00-6.364-6.364L12 7.636l-1.318-1.318a4.5 4.5 0 00-6.364 0z"
							/>
						</svg>
					);
				})}
			</div>
		</div>
	);
}
