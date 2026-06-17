"use client";

import confetti from "canvas-confetti";

export function fireConfetti() {
	confetti({
		particleCount: 110,
		spread: 90,
		origin: { y: 0.65 },
		colors: ["#E9A8C6", "#FFB6D9", "#A7F3D0", "#FFF4F7"]
	});
}

export default function ConfettiEffect() {
	return null;
}
