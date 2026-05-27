#pragma once
#include "2DPendentLib/Thread.h"
#include <vector>
#include <functional>
#include <thread>
#include <mutex>

//Exemple Méthode Cas d'usage
// 1 AddTaskChargement de texture
// 2 RunAsyncPathfinding A* + callback IA
// 3 AddTaskWithValue<int>Envoi paquet réseau
// 4 AddTaskWithValue<string>Sauvegarde fichier
// 5 Plusieurs AddTaskWithValueChargement parallèle d'assets
// 6 UpdateValue / GetValueScore partagé entre threads

class ThreadManager
{
private:

	//Liste de threads pour stocker les threads actuellement actifs, cela permet de les gérer et de les nettoyer correctement lors de la destruction du ThreadManager
	std::vector<std::unique_ptr<Thread>> m_threads;
	mutable std::mutex m_threadMutex; // protège m_threads uniquement

	// Données partagées : tu peux en ajouter d’autres plus tard
	int m_value = 0;
	std::vector<int> m_logs;
	mutable std::mutex m_dataMutex; // protège m_value et m_logs uniquement

	//Constructeur par défaut de ThreadManager
	ThreadManager();

public:

	// Supprime copie et move = impossible de dupliquer le singleton
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	ThreadManager(ThreadManager&&) = delete;
	ThreadManager& operator=(ThreadManager&&) = delete;

	//Destructeur de ThreadManager, il doit s'assurer que tous les threads sont correctement arrêtés
	//et nettoyés pour éviter les fuites de mémoire ou les comportements indéfinis
	~ThreadManager();

	static ThreadManager* Get();

    /*
        RunAsync(task, onDone)
        C'est quoi  : Lance une tache dans un thread, puis execute un callback a la fin.
        Pourquoi    : Utile pour declencher une action apres un chargement ou un calcul.
        Exemple     : ThreadManager::Get()->RunAsync(
                          []() { Resultat = CalculerAStar(debut, fin); },
                          []() { IA.ActiverDeplacement(); }
                      );
    */
    void RunAsync(const std::function<void()>& task, const std::function<void()>& onDone);

    /*
        AddTask(function)
        C'est quoi  : Lance une tache simple dans un nouveau thread.
        Pourquoi    : Ideal pour les operations longues qui ne bloquent pas le jeu.
        Exemple     : ThreadManager::Get()->AddTask([]() {
                          ChargerTexture("hero.png");
                      });
    */
    void AddTask(const std::function<void()> function);

    /*
        JoinAll()
        C'est quoi  : Attend que tous les threads actifs aient termine.
        Pourquoi    : Indispensable avant de fermer le jeu ou de changer de scene
                      pour eviter qu'un thread accede a de la memoire deja liberee.
        Exemple     : ThreadManager::Get()->JoinAll();
    */
    void JoinAll();

    /*
        UpdateValue(delta)
        C'est quoi  : Modifie m_value de facon thread-safe.
        Pourquoi    : Plusieurs threads peuvent modifier la valeur en meme temps,
                      le mutex garantit qu'un seul a la fois y accede.
        Exemple     : ThreadManager::Get()->UpdateValue(10);
    */
    void UpdateValue(int delta);

    /*
        LogWork(value)
        C'est quoi  : Ajoute une valeur dans m_logs de facon thread-safe.
        Pourquoi    : Permet de logger depuis plusieurs threads sans corrompre la liste.
        Exemple     : ThreadManager::Get()->LogWork(monThreadId);
    */
    void LogWork(int value);

    /*
        GetValue()
        C'est quoi  : Lit m_value de facon thread-safe.
        Pourquoi    : Meme une lecture doit etre protegee si un autre thread
                      peut ecrire en meme temps.
        Exemple     : int score = ThreadManager::Get()->GetValue();
    */
    int GetValue() const;

    /*
        AddTaskWithValue<T>(function, value)
        C'est quoi  : Lance une tache dans un thread en lui passant une valeur de type T.
        Pourquoi    : Evite les captures lambda manuelles quand on veut passer
                      une valeur simple a un thread.
        Exemple     : ThreadManager::Get()->AddTaskWithValue<int>(
                          [](int id) { Reseau.Envoyer(id); }, 42
                      );
    */
    template<typename T>
    void AddTaskWithValue(const std::function<void(T)> function, T value);
};

#include "ThreadManager.inl"