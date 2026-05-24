#include "Transition.h"

template<typename T>
Transition<T>::Transition()
{
	m_conditions.reserve(6); // Réserver de la mémoire pour 6 conditions afin d'éviter les reallocations fréquentes lors de l'ajout de conditions
}

template<typename T>
Transition<T>::~Transition()
{
	for (int i = 0; i < m_conditions.size(); ++i)
	{
		delete m_conditions[i];
	}
	m_conditions.clear();
}

template<typename T>
void Transition<T>::AddCondition(Condition<T>* condition)
{
	m_conditions.push_back(condition);
	
}

template<typename T>
int Transition<T>::TryTransition(T* type)
{
	for (int i = 0; i < m_conditions.size(); ++i)
	{
		if (m_conditions[i]->InternalTest(type) == false)
		{
			return -1;
		}
	}
	return m_targetState;
	
}
