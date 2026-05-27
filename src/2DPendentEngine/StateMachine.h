#pragma once
#include <vector>
#include "StateBase.h"
#include "Transition.h"

template<typename T>
class StateMachine {
private:

	std::vector<StateBase<T>*> m_states;
	std::vector<std::vector<Transition<T>*>> m_transitions;

	int m_currentState = 0;

	bool m_freezeBehaviours = false;

public:

	StateMachine();
	~StateMachine() = default;

	void AddState(StateBase<T>* newState);

	void Update(T* type, float dt);

	void TryTransitionTo(T* type, int nextState);

	Transition<T>* AddTransition(int fromState, int toState);

	int GetState() const { return m_currentState; }

	void ToggleFreeze() { m_freezeBehaviours = !m_freezeBehaviours; }
};

#include "StateMachine.inl"