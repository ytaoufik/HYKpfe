import Link from "next/link";

export function Navbar() {
	return (
		<header className="sticky top-0 z-50 border-b border-primary/20 bg-bg/85 backdrop-blur">
			<nav className="mx-auto flex w-full max-w-6xl items-center justify-between px-4 py-4">
				<Link href="/" className="text-xl font-black tracking-widest text-primary">
					H.Y.K
				</Link>
				<div className="flex items-center gap-3 text-sm font-semibold">
					<Link href="/teacher" className="rounded-lg px-3 py-2 hover:bg-card">Teacher</Link>
					<Link href="/join" className="rounded-lg px-3 py-2 hover:bg-card">Join</Link>
				</div>
			</nav>
		</header>
	);
}
