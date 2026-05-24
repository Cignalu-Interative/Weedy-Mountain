#pragma once
#include <vector>
#include "Condition.h"

template<typename T>
class Transition
{
	std::vector<Condition<T>*> m_conditions; // Liste de pointeurs vers les conditions associées à la transition
	int m_targetState; // État cible de la transition

public:

	Transition();
	~Transition();

	//Fonction pour ajouter une transition avec un état cible donné
	Transition(int targetState) { m_targetState = targetState; }

	//Fonction pour ajouter une condition à la transition
	void AddCondition(Condition<T>* condition);
	
	//Fonction pour essayer de réaliser la transition en fonction des conditions
	int TryTransition(T* type);
};

#include "Transition.inl"