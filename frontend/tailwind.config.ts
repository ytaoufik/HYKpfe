import type { Config } from "tailwindcss";

const config: Config = {
	content: [
		"./app/**/*.{js,ts,jsx,tsx,mdx}",
		"./components/**/*.{js,ts,jsx,tsx,mdx}",
		"./lib/**/*.{js,ts,jsx,tsx,mdx}"
	],
	theme: {
		extend: {
			fontFamily: {
				inter: ["Inter", "ui-sans-serif", "system-ui", "-apple-system", "Segoe UI", "sans-serif"]
			},
			colors: {
				bg: "#0F0A12",
				card: "#1C1223",
				primary: "#E9A8C6",
				secondary: "#FFB6D9",
				accent: "#D27CA8",
				success: "#A7F3D0",
				danger: "#FB7185",
				text: "#FFF4F7",
				warning: "#FBBF24"
			},
			boxShadow: {
				glow: "0 0 32px rgba(233,168,198,0.35)",
				"glow-sm": "0 0 12px rgba(233,168,198,0.2)",
				"glow-lg": "0 0 48px rgba(233,168,198,0.45)",
				"glow-accent": "0 0 32px rgba(210,124,168,0.4)",
				"glow-success": "0 0 24px rgba(167,243,208,0.3)",
				"glow-danger": "0 0 24px rgba(251,113,133,0.3)"
			},
			backdropBlur: {
				xs: "2px",
				"2xl": "40px",
				"3xl": "64px"
			},
			animation: {
				floatIn: "floatIn 0.6s ease-out",
				pulseSoft: "pulseSoft 1.5s ease-in-out infinite",
				shake: "shake 0.35s ease-in-out",
				fadeInUp: "fadeInUp 0.6s ease-out forwards",
				scoreFloat: "scoreFloat 1.2s ease-out forwards",
				countdownRing: "countdownRing linear forwards",
				pulseGlow: "pulseGlow 2s ease-in-out infinite",
				shimmer: "shimmer 2s linear infinite",
				slideInRight: "slideInRight 0.4s ease-out forwards",
				bounceIn: "bounceIn 0.6s ease-out forwards",
				heartbeat: "heartbeat 1.2s ease-in-out infinite",
				breakHeart: "breakHeart 0.6s ease-out forwards",
				levelUp: "levelUp 0.8s ease-out",
				gradientShift: "gradientShift 3s ease infinite",
				float: "float 6s ease-in-out infinite",
				floatSlow: "floatSlow 8s ease-in-out infinite",
				spin: "spin 1s linear infinite",
				slideUp: "slideUp 0.5s ease-out forwards",
				countdownBounce: "countdownBounce 1s ease-out forwards"
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
				},
				fadeInUp: {
					"0%": { opacity: "0", transform: "translateY(24px)" },
					"100%": { opacity: "1", transform: "translateY(0)" }
				},
				scoreFloat: {
					"0%": { opacity: "1", transform: "translateY(0) scale(1)" },
					"50%": { opacity: "1", transform: "translateY(-30px) scale(1.2)" },
					"100%": { opacity: "0", transform: "translateY(-60px) scale(1)" }
				},
				countdownRing: {
					from: { strokeDashoffset: "0" },
					to: { strokeDashoffset: "283" }
				},
				pulseGlow: {
					"0%, 100%": { boxShadow: "0 0 8px rgba(233,168,198,0.2)" },
					"50%": { boxShadow: "0 0 24px rgba(233,168,198,0.5)" }
				},
				shimmer: {
					"0%": { backgroundPosition: "-200% center" },
					"100%": { backgroundPosition: "200% center" }
				},
				slideInRight: {
					"0%": { opacity: "0", transform: "translateX(80px)" },
					"100%": { opacity: "1", transform: "translateX(0)" }
				},
				bounceIn: {
					"0%": { opacity: "0", transform: "scale(0.3)" },
					"50%": { opacity: "1", transform: "scale(1.08)" },
					"70%": { transform: "scale(0.95)" },
					"100%": { transform: "scale(1)" }
				},
				heartbeat: {
					"0%, 100%": { transform: "scale(1)" },
					"14%": { transform: "scale(1.25)" },
					"28%": { transform: "scale(1)" },
					"42%": { transform: "scale(1.15)" },
					"70%": { transform: "scale(1)" }
				},
				breakHeart: {
					"0%": { transform: "scale(1) rotate(0deg)", opacity: "1" },
					"30%": { transform: "scale(1.2) rotate(-5deg)" },
					"60%": { transform: "scale(0.6) rotate(10deg)", opacity: "0.5" },
					"100%": { transform: "scale(0) rotate(20deg)", opacity: "0" }
				},
				levelUp: {
					"0%": { transform: "scale(1)", textShadow: "0 0 0px transparent" },
					"30%": { transform: "scale(1.3)", textShadow: "0 0 20px rgba(167,243,208,0.8)" },
					"60%": { transform: "scale(0.95)" },
					"100%": { transform: "scale(1)", textShadow: "0 0 0px transparent" }
				},
				gradientShift: {
					"0%": { backgroundPosition: "0% 50%" },
					"50%": { backgroundPosition: "100% 50%" },
					"100%": { backgroundPosition: "0% 50%" }
				},
				float: {
					"0%, 100%": { transform: "translateY(0px)" },
					"50%": { transform: "translateY(-20px)" }
				},
				floatSlow: {
					"0%, 100%": { transform: "translateY(0px) translateX(0px)" },
					"33%": { transform: "translateY(-15px) translateX(8px)" },
					"66%": { transform: "translateY(-8px) translateX(-5px)" }
				},
				slideUp: {
					"0%": { opacity: "0", transform: "translateY(40px)" },
					"100%": { opacity: "1", transform: "translateY(0)" }
				},
				countdownBounce: {
					"0%": { opacity: "0", transform: "scale(0.3)" },
					"50%": { opacity: "1", transform: "scale(1.15)" },
					"80%": { transform: "scale(0.95)" },
					"100%": { opacity: "0", transform: "scale(0.8)" }
				}
			}
		}
	},
	plugins: []
};

export default config;
