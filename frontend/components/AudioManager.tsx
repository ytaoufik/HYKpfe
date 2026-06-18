"use client";

import { useEffect, useState, useRef } from "react";

// Safe AudioContext helper
let globalAudioContext: AudioContext | null = null;
function getAudioContext() {
	if (typeof window === "undefined") return null;
	if (!globalAudioContext) {
		globalAudioContext = new (window.AudioContext || (window as any).webkitAudioContext)();
	}
	return globalAudioContext;
}

// Check if sound is muted in localStorage
export function isMuted(): boolean {
	if (typeof window === "undefined") return false;
	return localStorage.getItem("hyk_sound_muted") === "true";
}

export function setMutedState(muted: boolean) {
	if (typeof window === "undefined") return;
	localStorage.setItem("hyk_sound_muted", muted ? "true" : "false");
	window.dispatchEvent(new Event("hyk_muted_change"));
}

function playTone(
	freqs: number[],
	times: number[],
	durations: number[],
	type: OscillatorType,
	gainSequence: number[]
) {
	if (isMuted()) return;
	const ctx = getAudioContext();
	if (!ctx) return;

	if (ctx.state === "suspended") {
		ctx.resume();
	}

	const now = ctx.currentTime;

	freqs.forEach((freq, index) => {
		const osc = ctx.createOscillator();
		const gainNode = ctx.createGain();
		
		osc.type = type;
		osc.frequency.setValueAtTime(freq, now + times[index]);

		const baseGain = gainSequence[index] !== undefined ? gainSequence[index] : 0.08;
		const duration = durations[index] || 0.15;

		gainNode.gain.setValueAtTime(0, now + times[index]);
		// Attack
		gainNode.gain.linearRampToValueAtTime(baseGain, now + times[index] + 0.01);
		// Decay
		gainNode.gain.exponentialRampToValueAtTime(0.0001, now + times[index] + duration);

		osc.connect(gainNode);
		gainNode.connect(ctx.destination);

		osc.start(now + times[index]);
		osc.stop(now + times[index] + duration);
	});
}

export function useAudioManager() {
	const [muted, setMuted] = useState(false);

	useEffect(() => {
		setMuted(isMuted());
		const handleMutedChange = () => setMuted(isMuted());
		window.addEventListener("hyk_muted_change", handleMutedChange);
		return () => window.removeEventListener("hyk_muted_change", handleMutedChange);
	}, []);

	return {
		muted,
		toggleMute() {
			const nextMuted = !muted;
			setMuted(nextMuted);
			setMutedState(nextMuted);
		},
		correct() {
			// Ascending two-tone chime
			playTone([523.25, 659.25], [0, 0.08], [0.2, 0.25], "triangle", [0.08, 0.08]);
		},
		wrong() {
			// Low buzzer with pitch slide
			playTone([180, 150], [0, 0.08], [0.15, 0.25], "sawtooth", [0.06, 0.06]);
		},
		click() {
			// Soft organic click
			playTone([800], [0], [0.05], "sine", [0.04]);
		},
		streak() {
			// Rising three-tone fanfare
			playTone([523.25, 659.25, 783.99], [0, 0.08, 0.16], [0.15, 0.15, 0.35], "triangle", [0.07, 0.07, 0.09]);
		},
		levelUp() {
			// Triumphant ascending scale
			const scale = [261.63, 329.63, 392.00, 523.25, 659.25, 783.99, 1046.50];
			const times = scale.map((_, i) => i * 0.06);
			const durations = scale.map(() => 0.2);
			const gains = scale.map(() => 0.06);
			playTone(scale, times, durations, "sine", gains);
		},
		gameOver() {
			// Sad descending tones
			playTone([392.00, 349.23, 311.13, 261.63], [0, 0.15, 0.3, 0.45], [0.2, 0.2, 0.25, 0.4], "sawtooth", [0.05, 0.05, 0.05, 0.05]);
		},
		victory() {
			// Triumphant melody
			const notes = [523.25, 523.25, 523.25, 523.25, 659.25, 587.33, 659.25, 783.99];
			const times = [0, 0.12, 0.24, 0.36, 0.48, 0.6, 0.72, 0.84];
			const durations = [0.15, 0.15, 0.15, 0.25, 0.15, 0.15, 0.15, 0.5];
			const gains = notes.map(() => 0.07);
			playTone(notes, times, durations, "triangle", gains);
		},
		countdown() {
			// Short pop for 3, 2, 1
			playTone([600], [0], [0.1], "sine", [0.08]);
		},
		tick() {
			// Soft clock tick
			playTone([1200], [0], [0.03], "sine", [0.02]);
		}
	};
}

export default function AudioManager() {
	return null;
}
