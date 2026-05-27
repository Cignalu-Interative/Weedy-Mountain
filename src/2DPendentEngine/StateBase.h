#pragma once

template <typename T>
class StateBase
{
public:

	//Entrée dans l'état, actions à effectuer lors de l'entrée dans l'état
	virtual void Start(T* type) = 0;
	//Mise à jour de l'état, appelée à chaque frame
	virtual void Update(T* type, float dt) = 0;
	//Sortie de l'état, actions à effectuer lors de la sortie de l'état
	virtual void End(T* type) = 0;

	//Vérification si la transition vers cet état est possible à partir de l'état actuel, retourne true si la transition est autorisée, false sinon
	virtual bool CanTransitionFrom(T* type, int currentState) { return true; };
};