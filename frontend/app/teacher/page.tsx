"use client";

import { useState } from "react";
import { generateQuiz } from "../../lib/api";
import { QuizCard } from "../../components/QuizCard";

export default function TeacherPage() {
	const [input, setInput] = useState("");
	const [loading, setLoading] = useState(false);
	const [error, setError] = useState("");
	const [result, setResult] = useState<{ code: string; title: string } | null>(null);

	async function onGenerate() {
		setLoading(true);
		setError("");
		setResult(null);
		try {
			const data = await generateQuiz(input);
			setResult({ code: data.game_code, title: data.quiz.title });
		} catch (e) {
			setError(e instanceof Error ? e.message : "Failed to generate quiz");
		} finally {
			setLoading(false);
		}
	}

	return (
		<section className="space-y-6">
			<h2 className="text-3xl font-extrabold">Teacher Console</h2>
			<QuizCard>
				<label className="mb-2 block text-sm text-text/80">Subject, lesson, or pasted text</label>
				<textarea
					className="h-44 w-full rounded-xl border border-primary/30 bg-bg/70 p-4 text-text outline-none focus:border-primary"
					value={input}
					onChange={(e) => setInput(e.target.value)}
					placeholder="Paste a chapter summary or enter a topic"
				/>
				<button
					onClick={onGenerate}
					disabled={loading || !input.trim()}
					className="mt-4 rounded-xl bg-primary px-5 py-3 font-bold text-bg disabled:opacity-50"
				>
					{loading ? "Generating..." : "Generate 10 Questions"}
				</button>
				{error && <p className="mt-3 text-danger">{error}</p>}
				{result && (
					<div className="mt-4 rounded-xl border border-success/40 bg-success/10 p-4">
						<p className="font-semibold">Quiz ready: {result.title}</p>
						<p className="text-lg font-black tracking-widest text-secondary">Code: {result.code}</p>
					</div>
				)}
			</QuizCard>
		</section>
	);
}
