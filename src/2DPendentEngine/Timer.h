#pragma once
#include <chrono>
#include <thread>
//Version 4.0.0

class Timer
{
private:

	// Temps du dernier GetElapsedTime() ou du constructeur
	std::chrono::steady_clock::time_point m_currentTime;
	// Temps du GetElapsedTime() précédent
	std::chrono::steady_clock::time_point m_lastTime;
	// Temps écoulé entre le temps actuel et le temps précédent en float
	double m_elapsed;

public:

	//Constructeur de Timer
	Timer() = default;
	//Destructeur de Timer
	~Timer() = default;

	void SetCurrentTime();

	//Obtenir le temps actuelle
	std::chrono::steady_clock::time_point GetCurrentTime(); 
	//Obtenir la différence entre deux frame
	double GetElapsed();

	void DebugTime(); //Afichage du temps en mode debug

	//Dormir pendant un certain nombre de nanosecondes (utilisé pour limiter le nombre de frame par seconde)
	void Sleep(float p_SecForSleep);
};