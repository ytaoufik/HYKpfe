import type { Metadata } from "next";
import "../styles/globals.css";
import { Navbar } from "../components/Navbar";

export const metadata: Metadata = {
	title: "H.Y.K - AI Quiz Battle",
	description: "How You Know - AI Powered Quiz Battle"
};

export default function RootLayout({ children }: { children: React.ReactNode }) {
	return (
		<html lang="en">
			<body className="min-h-screen bg-bg text-text">
				<Navbar />
				<main className="mx-auto w-full max-w-6xl px-4 pb-16 pt-8">{children}</main>
			</body>
		</html>
	);
}
