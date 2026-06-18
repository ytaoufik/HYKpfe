"use client";

import confetti from "canvas-confetti";

export function fireConfetti() {
	confetti({
		particleCount: 100,
		spread: 70,
		origin: { y: 0.6 },
		colors: ["#E9A8C6", "#FFB6D9", "#A7F3D0", "#FFF4F7"]
	});
}

export function fireVictoryConfetti() {
	const duration = 3 * 1000;
	const end = Date.now() + duration;

	const frame = () => {
		confetti({
			particleCount: 5,
			angle: 60,
			spread: 55,
			origin: { x: 0 },
			colors: ["#E9A8C6", "#FFB6D9", "#A7F3D0", "#FFF4F7"]
		});
		confetti({
			particleCount: 5,
			angle: 120,
			spread: 55,
			origin: { x: 1 },
			colors: ["#E9A8C6", "#FFB6D9", "#A7F3D0", "#FFF4F7"]
		});

		if (Date.now() < end) {
			requestAnimationFrame(frame);
		}
	};
	frame();
}

export function fireStreakConfetti() {
	// Smaller burst with gold-ish theme
	confetti({
		particleCount: 50,
		spread: 60,
		origin: { y: 0.75 },
		colors: ["#FBBF24", "#F59E0B", "#FFF4F7"]
	});
}

export default function ConfettiEffect() {
	return null;
}
