#include "StateMachine.h"

template<typename T>
StateMachine<T>::StateMachine()
{
	m_states.reserve(10); // Réserver de la mémoire pour 10 états afin d'éviter les reallocations fréquentes lors de l'ajout d'états
	m_transitions.reserve(10); // Réserver de la mémoire pour 10 états afin d'éviter les reallocations fréquentes lors de l'ajout d'états
}

template <typename T>
void StateMachine<T>::AddState(StateBase<T>* newState)
{
    m_states.push_back(newState);
    m_transitions.resize(m_states.size());
    m_transitions.back().reserve(4);
}

template<typename T>
void StateMachine<T>::Update(T* type, float dt)
{
	if (m_freezeBehaviours == false)
	{
		m_states[m_currentState]->Update(type, dt);
	}
	std::vector<Transition<T>*>& transitions = m_transitions[m_currentState];
	for (int i = 0; i < transitions.size(); ++i)
	{
		int nextState = transitions[i]->TryTransition(type);
		if (nextState == -1)
		{
			continue;
		}
		TryTransitionTo(type, nextState);
		break;
	}
}

template<typename T>
void StateMachine<T>::TryTransitionTo(T* type, int nextState)
{
	if (m_states[(int)nextState]->CanTransitionFrom(type, (int)m_currentState) == false)
	{
		return;
	}
	if (m_freezeBehaviours == false)
	{
		m_states[(int)m_currentState]->End(type);
	}
	m_currentState = nextState;
	if (m_freezeBehaviours == false)
	{
		m_states[(int)m_currentState]->Start(type);
	}
}

template<typename T>
Transition<T>* StateMachine<T>::AddTransition(int fromState, int toState)
{
	Transition<T>* transition = new Transition<T>(toState);
	m_transitions[fromState].push_back(transition);

	return transition;
}