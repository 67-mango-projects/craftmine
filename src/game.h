#pragma once



class Game {
private:
	inline static double m_deltaTime;
public:
	static double getDeltaTime() { return m_deltaTime; }
	int run();
private:
	void updateStats();

};