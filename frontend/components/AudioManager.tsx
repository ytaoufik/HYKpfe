"use client";

import { useRef } from "react";

function playTone(ctx: AudioContext, freq: number, duration: number, type: OscillatorType) {
	const osc = ctx.createOscillator();
	const gain = ctx.createGain();
	osc.type = type;
	osc.frequency.value = freq;
	gain.gain.value = 0.06;
	osc.connect(gain);
	gain.connect(ctx.destination);
	osc.start();
	setTimeout(() => osc.stop(), duration);
}

export function useAudioManager() {
	const ctxRef = useRef<AudioContext | null>(null);

	function ensureCtx() {
		if (!ctxRef.current) {
			ctxRef.current = new AudioContext();
		}
		return ctxRef.current;
	}

	return {
		correct() {
			const ctx = ensureCtx();
			playTone(ctx, 780, 130, "triangle");
			setTimeout(() => playTone(ctx, 1040, 120, "triangle"), 80);
		},
		wrong() {
			const ctx = ensureCtx();
			playTone(ctx, 220, 220, "sawtooth");
		},
		click() {
			const ctx = ensureCtx();
			playTone(ctx, 520, 70, "square");
		}
	};
}

export default function AudioManager() {
	return null;
}
