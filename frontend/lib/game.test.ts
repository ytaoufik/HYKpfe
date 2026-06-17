import { describe, expect, it } from "vitest";
import { bonusPerfectQuiz, calculateLevel, calculateScore, calculateXp, isPerfectQuiz } from "./game";

describe("game formulas", () => {
  it("calculates score with speed bonus", () => {
    expect(calculateScore(20, 1, true)).toBe(140);
  });

  it("applies streak bonus every 3", () => {
    expect(calculateScore(10, 3, true)).toBe(320);
  });

  it("returns zero on wrong answers", () => {
    expect(calculateScore(25, 3, false)).toBe(0);
  });

  it("calculates xp and levels", () => {
    expect(calculateXp(320, true)).toBeGreaterThanOrEqual(20);
    expect(calculateLevel(1000)).toBe(3);
  });

  it("handles perfect quiz bonus", () => {
    expect(isPerfectQuiz(10, 10)).toBe(true);
    expect(bonusPerfectQuiz()).toBe(1000);
  });
});