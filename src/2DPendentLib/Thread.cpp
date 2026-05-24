#include "pch.h"
#include "Thread.h"

Thread::~Thread()
{
	if (m_thread.joinable()) //Vérifier si le thread est encore joinable avant de tenter de le joindre pour éviter des comportements indéfinis
	{
		m_thread.join(); //Joindre le thread pour s'assurer qu'il se termine correctement avant de détruire l'objet Thread
	}
}

void Thread::Join()
{
	if (m_thread.joinable()) //Vérifier si le thread est encore joinable avant de tenter de le joindre pour éviter des comportements indéfinis
	{
		m_thread.join(); //Joindre le thread pour s'assurer qu'il se termine correctement avant de continuer
	}
}