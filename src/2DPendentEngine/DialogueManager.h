#pragma once
#include "2DPendentLib/Text.h"

class Dialogue;
class DialogueManager
{

private :

	std::unordered_map<std::string, size_t> m_dialoguesMap; // Map pour stocker les dialogues chargées, indexées par leur chemin d'accès
	std::vector<Dialogue*> m_dialoguesVector; // Vector pour stocker les dialogues chargées, utilisé pour itérer plus rapidement lors de la libération des ressources
	std::vector<std::string> m_dialoguesIdsVector; // Vector pour stocker les identifiants des dialogues chargées, utilisé pour itérer plus rapidement lors de la libération des ressources

public :

	//Singleton de DialogueManager
	static DialogueManager* Get();

	// Constructeur par défaut de AssetManager
	DialogueManager();
	// Destructeur de AssetManager
	~DialogueManager();

	// Ajoute un dialogue à partir d'un répertoire JSON contenant les chemins d'accès des dialogues
	void AddDialogueByRepertory(const std::string& p_path);

	// Ajoute un dialogue à partir d'un chemin d'accès
	void AddDialogueByPath(const std::string& p_path);

	// Obtient une texture à partir de son identifiant
	std::string* GetDialogue(const std::string& p_name);

};

