import type { Metadata } from "next";
import { Inter } from "next/font/google";
import "../styles/globals.css";
import { Navbar } from "../components/Navbar";

const inter = Inter({
	subsets: ["latin"],
	weight: ["300", "400", "500", "600", "700", "800", "900"],
	display: "swap",
	variable: "--font-inter"
});

export const metadata: Metadata = {
	title: "H.Y.K — AI Quiz Battle",
	description:
		"How You Know — Generate AI-powered quizzes, battle in real time with lives, streaks, XP, and achievements. The ultimate classroom gaming experience.",
	keywords: ["quiz", "AI", "battle", "education", "gamification", "leaderboard"],
	authors: [{ name: "H.Y.K Team" }],
	themeColor: "#0F0A12",
	openGraph: {
		title: "H.Y.K — AI Quiz Battle",
		description:
			"Generate AI-powered quizzes and battle in real time with lives, streaks, XP, and achievements.",
		type: "website",
		locale: "en_US",
		siteName: "H.Y.K"
	},
	robots: "index, follow"
};

export default function RootLayout({ children }: { children: React.ReactNode }) {
	return (
		<html lang="en" className={inter.variable}>
			<body className="min-h-screen bg-bg font-inter text-text antialiased">
				<Navbar />
				<main className="relative mx-auto w-full max-w-7xl px-4 pb-16 pt-4 sm:px-6 lg:px-8">
					{children}
				</main>
			</body>
		</html>
	);
}
