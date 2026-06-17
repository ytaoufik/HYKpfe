import { QuizQuestion } from "../lib/api";

type Props = {
	question: QuizQuestion;
	onSelect: (idx: number) => void;
	selected: number | null;
	reveal: boolean;
	wrongFlash: boolean;
};

export function QuestionCard({ question, onSelect, selected, reveal, wrongFlash }: Props) {
	return (
		<div className={`rounded-2xl border border-primary/25 bg-card p-6 ${wrongFlash ? "animate-shake" : ""}`}>
			<h3 className="text-xl font-bold">{question.question}</h3>
			<div className="mt-4 grid gap-3">
				{question.options.map((opt, idx) => {
					const isCorrect = idx === question.correct_answer;
					const isSelected = selected === idx;
					const cls = reveal
						? isCorrect
							? "border-success bg-success/20"
							: isSelected
								? "border-danger bg-danger/20"
								: "border-primary/20"
						: "border-primary/20 hover:border-primary hover:bg-primary/10";
					return (
						<button
							key={`${question.question}-${idx}`}
							onClick={() => onSelect(idx)}
							disabled={reveal}
							className={`rounded-xl border px-4 py-3 text-left transition ${cls}`}
						>
							<span className="font-bold text-secondary">{String.fromCharCode(65 + idx)}.</span> {opt}
						</button>
					);
				})}
			</div>
			{reveal && <p className="mt-4 text-sm text-text/80">{question.explanation}</p>}
		</div>
	);
}
