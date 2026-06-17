export function QuizCard({ children }: { children: React.ReactNode }) {
	return <div className="rounded-3xl border border-primary/20 bg-card/85 p-6 shadow-glow">{children}</div>;
}
