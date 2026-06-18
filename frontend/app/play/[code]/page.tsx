"use client";

import { useParams, useRouter, useSearchParams } from "next/navigation";
import { useEffect, useMemo, useState } from "react";
import { fetchLeaderboard, fetchQuiz, submitScore } from "../../../lib/api";
import { bonusPerfectQuiz, calculateLevel, calculateScore, calculateXp, isPerfectQuiz, resolveAchievements } from "../../../lib/game";
import { supabase } from "../../../lib/supabase";
import { AchievementPopup } from "../../../components/AchievementPopup";
import { fireConfetti } from "../../../components/ConfettiEffect";
import { GameOverScreen } from "../../../components/GameOverScreen";
import { Leaderboard } from "../../../components/Leaderboard";
import { LivesDisplay } from "../../../components/LivesDisplay";
import { LoadingScreen } from "../../../components/LoadingScreen";
import { QuestionCard } from "../../../components/QuestionCard";
import { ScoreCounter } from "../../../components/ScoreCounter";
import { Timer } from "../../../components/Timer";
import { useAudioManager } from "../../../components/AudioManager";
import { VictoryScreen } from "../../../components/VictoryScreen";
import { QuizQuestion } from "../../../lib/api";

const QUESTION_TIME = 30;

type SessionState = {
	idx: number;
	score: number;
	streak: number;
	lives: number;
	xp: number;
	level: number;
	correctCount: number;
};

const initialState: SessionState = {
	idx: 0,
	score: 0,
	streak: 0,
	lives: 3,
	xp: 0,
	level: 1,
	correctCount: 0
};

export default function PlayPage() {
	const params = useParams<{ code: string }>();
	const router = useRouter();
	const search = useSearchParams();
	const playerName = search.get("player") || (typeof window !== "undefined" ? localStorage.getItem("hyk_player_name") || "Player" : "Player");
	const code = (params?.code || "").toUpperCase();
	const sessionKey = `hyk_session_${code}`;
	const audio = useAudioManager();

	const [questions, setQuestions] = useState<QuizQuestion[]>([]);
	const [loading, setLoading] = useState(true);
	const [state, setState] = useState<SessionState>(initialState);
	const [remaining, setRemaining] = useState(QUESTION_TIME);
	const [selected, setSelected] = useState<number | null>(null);
	const [reveal, setReveal] = useState(false);
	const [wrongFlash, setWrongFlash] = useState(false);
	const [achievement, setAchievement] = useState<{ key: string; title: string; description: string } | null>(null);
	const [board, setBoard] = useState<Array<{ player_name: string; score: number; xp: number }>>([]);

	useEffect(() => {
		fetchQuiz(code)
			.then((data) => {
				setQuestions(data.quiz.questions);
				const raw = localStorage.getItem(sessionKey);
				if (raw) {
					const restored = JSON.parse(raw) as SessionState;
					setState(restored);
				}
			})
			.finally(() => setLoading(false));
	}, [code, sessionKey]);

	useEffect(() => {
		if (loading || reveal) return;
		if (state.idx >= questions.length || state.lives <= 0) return;

		const timer = setInterval(() => {
			setRemaining((prev) => {
				if (prev <= 1) {
					onAnswer(-1);
					return QUESTION_TIME;
				}
				return prev - 1;
			});
		}, 1000);
		return () => clearInterval(timer);
	}, [loading, reveal, state.idx, state.lives, questions.length]);

	useEffect(() => {
		if (!loading) {
			localStorage.setItem(sessionKey, JSON.stringify(state));
		}
	}, [state, sessionKey, loading]);

	useEffect(() => {
		fetchLeaderboard(code)
			.then((d) => setBoard(d.leaderboard))
			.catch(() => setBoard([]));
	}, [code, state.score]);

	useEffect(() => {
		const channel = supabase
			.channel(`scores-${code}`)
			.on(
				"postgres_changes",
				{ event: "INSERT", schema: "public", table: "scores", filter: `game_code=eq.${code}` },
				() => {
					fetchLeaderboard(code)
						.then((d) => setBoard(d.leaderboard))
						.catch(() => undefined);
				}
			)
			.subscribe();

		return () => {
			supabase.removeChannel(channel);
		};
	}, [code]);

	const question = useMemo(() => questions[state.idx], [questions, state.idx]);

	async function onAnswer(answerIndex: number) {
		if (!question || reveal) return;

		setReveal(true);
		setSelected(answerIndex >= 0 ? answerIndex : null);
		const isCorrect = answerIndex === question.correct_answer;

		const nextStreak = isCorrect ? state.streak + 1 : 0;
		const delta = calculateScore(remaining, nextStreak, isCorrect);
		const nextScore = state.score + delta;
		const nextXp = state.xp + calculateXp(delta, isCorrect);
		const nextLevel = calculateLevel(nextXp);
		const nextLives = isCorrect ? state.lives : Math.max(0, state.lives - 1);
		const nextCorrect = isCorrect ? state.correctCount + 1 : state.correctCount;

		if (isCorrect) {
			audio.correct();
			fireConfetti();
		} else {
			audio.wrong();
			setWrongFlash(true);
			setTimeout(() => setWrongFlash(false), 320);
		}

		const partial: SessionState = {
			...state,
			score: nextScore,
			streak: nextStreak,
			xp: nextXp,
			level: nextLevel,
			lives: nextLives,
			correctCount: nextCorrect
		};

		await submitScore({
			game_code: code,
			player_name: playerName,
			score: partial.score,
			xp: partial.xp,
			streak: partial.streak,
			lives: partial.lives
		}).catch(() => undefined);

		const ach = resolveAchievements({
			streak: partial.streak,
			level: partial.level,
			perfect: false,
			correctCount: partial.correctCount,
			lives: partial.lives,
			totalLives: initialState.lives,
			totalQuestions: questions.length,
			answerTimeSeconds: QUESTION_TIME - remaining
		});
		if (ach.length > 0) {
			setAchievement(ach[0]);
			setTimeout(() => setAchievement(null), 1800);
		}

		setTimeout(async () => {
			const nextIdx = state.idx + 1;
			let finalScore = partial.score;

			if (nextIdx >= questions.length && isPerfectQuiz(partial.correctCount, questions.length)) {
				finalScore += bonusPerfectQuiz();
			}

			const nextState: SessionState = {
				...partial,
				idx: nextIdx,
				score: finalScore
			};

			await submitScore({
				game_code: code,
				player_name: playerName,
				score: nextState.score,
				xp: nextState.xp,
				streak: nextState.streak,
				lives: nextState.lives
			}).catch(() => undefined);

			setState(nextState);
			setRemaining(QUESTION_TIME);
			setSelected(null);
			setReveal(false);

			if (nextState.lives <= 0 || nextState.idx >= questions.length) {
				localStorage.setItem(sessionKey, JSON.stringify(nextState));
			}
		}, 1300);
	}

	if (loading) {
		return <LoadingScreen text="Loading battle arena..." />;
	}

	if (state.lives <= 0) {
		return <GameOverScreen code={code} />;
	}

	if (state.idx >= questions.length) {
		return <VictoryScreen code={code} />;
	}

	return (
		<section className="grid gap-6 lg:grid-cols-[1fr_320px]">
			<div className="space-y-4">
				<div className="rounded-2xl border border-primary/25 bg-card/85 p-4">
					<div className="mb-3 flex items-center justify-between gap-3">
						<p className="font-bold">Player: {playerName}</p>
						<LivesDisplay lives={state.lives} />
					</div>
					<ScoreCounter score={state.score} streak={state.streak} xp={state.xp} level={state.level} />
					<div className="mt-4">
						<Timer remaining={remaining} total={QUESTION_TIME} />
					</div>
					<div className="mt-3 h-2 overflow-hidden rounded-full bg-bg/60">
						<div
							className="h-full bg-secondary transition-all"
							style={{ width: `${((state.idx + 1) / Math.max(questions.length, 1)) * 100}%` }}
						/>
					</div>
					<p className="mt-2 text-sm text-text/70">
						Question {state.idx + 1} / {questions.length}
					</p>
				</div>
				{question && (
					<QuestionCard
						question={question}
						onSelect={(i) => {
							audio.click();
							onAnswer(i);
						}}
						selected={selected}
						reveal={reveal}
						wrongFlash={wrongFlash}
					/>
				)}
			</div>

			<aside className="space-y-3 rounded-2xl border border-primary/20 bg-card/80 p-4">
				<h3 className="text-xl font-black">Live Leaderboard</h3>
				<Leaderboard items={board} />
				<button
					className="w-full rounded-xl border border-primary px-4 py-2 text-sm font-bold text-primary"
					onClick={() => router.push(`/results/${code}`)}
				>
					Exit To Results
				</button>
			</aside>
			<AchievementPopup achievement={achievement} />
		</section>
	);
}
