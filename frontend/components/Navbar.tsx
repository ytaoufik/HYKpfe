"use client";

import Link from "next/link";
import { usePathname } from "next/navigation";
import { useState } from "react";

const navLinks: Array<{ href: string; label: string }> = [
	{ href: "/", label: "Home" },
	{ href: "/teacher", label: "Create Quiz" },
	{ href: "/join", label: "Join Game" }
];

export function Navbar() {
	const pathname = usePathname();
	const [muted, setMuted] = useState(false);

	return (
		<header className="sticky top-0 z-50 w-full border-b border-primary/10 glass">
			<nav className="mx-auto flex h-16 max-w-7xl items-center justify-between px-4 sm:px-6 lg:px-8">
				{/* Logo */}
				<Link href="/" className="group flex items-center gap-2">
					<span
						className="text-2xl font-black tracking-tight bg-gradient-to-r from-primary via-secondary to-accent bg-[length:200%_auto] bg-clip-text text-transparent animate-gradientShift"
						style={{ animationDuration: "4s" }}
					>
						H.Y.K
					</span>
					<span className="hidden text-xs font-medium text-text/40 sm:block">Quiz Battle</span>
				</Link>

				{/* Nav Links */}
				<div className="flex items-center gap-1 sm:gap-2">
					{navLinks.map((link) => {
						const isActive = pathname === link.href;
						return (
							<Link
								key={link.href}
								href={{ pathname: link.href }}
								className={`relative rounded-lg px-3 py-2 text-sm font-medium transition-all duration-200 hover:text-primary ${
									isActive ? "text-primary" : "text-text/60 hover:text-text/90"
								}`}
							>
								{link.label}
								{isActive && (
									<span className="absolute bottom-0 left-1/2 h-0.5 w-4/5 -translate-x-1/2 rounded-full bg-gradient-to-r from-primary to-accent" />
								)}
								<span className="absolute bottom-0 left-1/2 h-0.5 w-0 -translate-x-1/2 rounded-full bg-primary/50 transition-all duration-300 group-hover:w-4/5 hover:w-4/5" />
							</Link>
						);
					})}
				</div>

				{/* Sound Toggle */}
				<button
					onClick={() => setMuted((m) => !m)}
					className="flex h-9 w-9 items-center justify-center rounded-lg border border-primary/20 text-lg transition-all duration-200 hover:border-primary/40 hover:bg-primary/5 active:scale-95"
					aria-label={muted ? "Unmute sounds" : "Mute sounds"}
					title={muted ? "Unmute sounds" : "Mute sounds"}
				>
					{muted ? "🔇" : "🔊"}
				</button>
			</nav>
		</header>
	);
}
