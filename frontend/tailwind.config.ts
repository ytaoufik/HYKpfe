import type { Config } from "tailwindcss";

const config: Config = {
	content: [
		"./app/**/*.{js,ts,jsx,tsx,mdx}",
		"./components/**/*.{js,ts,jsx,tsx,mdx}",
		"./lib/**/*.{js,ts,jsx,tsx,mdx}"
	],
	theme: {
		extend: {
			colors: {
				bg: "#0F0A12",
				card: "#1C1223",
				primary: "#E9A8C6",
				secondary: "#FFB6D9",
				accent: "#D27CA8",
				success: "#A7F3D0",
				danger: "#FB7185",
				text: "#FFF4F7"
			},
			boxShadow: {
				glow: "0 0 32px rgba(233,168,198,0.35)"
			},
			animation: {
				floatIn: "floatIn 0.6s ease-out",
				pulseSoft: "pulseSoft 1.5s ease-in-out infinite",
				shake: "shake 0.35s ease-in-out"
			},
			keyframes: {
				floatIn: {
					"0%": { opacity: "0", transform: "translateY(12px) scale(0.96)" },
					"100%": { opacity: "1", transform: "translateY(0) scale(1)" }
				},
				pulseSoft: {
					"0%, 100%": { transform: "scale(1)" },
					"50%": { transform: "scale(1.04)" }
				},
				shake: {
					"0%": { transform: "translateX(0)" },
					"25%": { transform: "translateX(-6px)" },
					"50%": { transform: "translateX(6px)" },
					"75%": { transform: "translateX(-4px)" },
					"100%": { transform: "translateX(0)" }
				}
			}
		}
	},
	plugins: []
};

export default config;
