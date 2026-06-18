"use client";

import { useEffect, useRef } from "react";
import { useAudioManager } from "./AudioManager";

type TimerProps = {
	remaining: number;
	total: number;
	onExpire?: () => void;
};

export function Timer({ remaining, total, onExpire }: TimerProps) {
	const audio = useAudioManager();
	const prevRemaining = useRef(remaining);

	useEffect(() => {
		// Play tick sound on change if remaining <= 5 and > 0
		if (remaining !== prevRemaining.current) {
			if (remaining <= 5 && remaining > 0) {
				audio.countdown();
			} else if (remaining > 0) {
				audio.tick();
			}
			prevRemaining.current = remaining;
		}

		if (remaining === 0 && onExpire) {
			onExpire();
		}
	}, [remaining, audio, onExpire]);

	const radius = 40;
	const strokeWidth = 8;
	const circumference = 2 * Math.PI * radius;
	const progress = remaining / total;
	const strokeDashoffset = circumference - progress * circumference;

	// Color mapping
	let colorClass = "stroke-primary";
	let textClass = "text-text";
	let pulseClass = "";

	if (remaining <= 5) {
		colorClass = "stroke-danger";
		textClass = "text-danger font-black";
		pulseClass = "animate-heartbeat";
	} else if (remaining <= 15) {
		colorClass = "stroke-warning";
		textClass = "text-warning font-bold";
	}

	return (
		<div className={`relative flex h-24 w-24 items-center justify-center ${pulseClass}`}>
			{/* SVG Circle Timer */}
			<svg className="h-full w-full -rotate-90" viewBox="0 0 100 100">
				{/* Background ring */}
				<circle
					className="stroke-card/40"
					cx="50"
					cy="50"
					r={radius}
					strokeWidth={strokeWidth}
					fill="transparent"
				/>
				{/* Active depleting ring */}
				<circle
					className={`transition-all duration-1000 ease-linear ${colorClass}`}
					cx="50"
					cy="50"
					r={radius}
					strokeWidth={strokeWidth}
					fill="transparent"
					strokeDasharray={circumference}
					strokeDashoffset={strokeDashoffset}
					strokeLinecap="round"
				/>
			</svg>
			{/* Seconds in Center */}
			<div className="absolute flex flex-col items-center justify-center">
				<span className={`text-2xl tracking-tighter ${textClass}`}>
					{remaining}
				</span>
				<span className="text-[10px] uppercase tracking-wider text-text/40">Sec</span>
			</div>
		</div>
	);
}
