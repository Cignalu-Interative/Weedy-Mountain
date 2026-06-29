#pragma once
#include <SDL3/SDL.h>
#include <thread>

class Thread
{
private:
	//To Do ajouter std::blind pour éviter les problèmes de copie/move
	//Pointeur vers la fonction à exécuter dans le thread
	std::thread m_thread;

public:
	//Constructeur par défaut de Thread
	//std::function<type de la fonction(liste des parametres exemple int)>
	template<typename Func, typename... Args>
	Thread(Func&& func, Args&&... args);

	//Destructeur de Thread
	~Thread();

	//Fonction pour attendre la fin de l'exécution du thread avant de continuer,
	//elle doit être appelée avant de détruire l'objet Thread pour éviter des comportements indéfinis
	void Join();
};

#include "Thread.inl"