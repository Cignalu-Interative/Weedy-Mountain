#pragma once
#include "2DPendentLib/Text.h"
class DialogueManager
{
	struct Choice {
		std::string s_label;  // "Oui", "NON"
		int s_target;         // @3, @4
	};

	struct DialogNode {
		int s_id;
		std::string s_speaker;
		Text text;
		std::vector<Choice> s_choices;  // vide = pas de question
	};

private :

	std::unordered_map<int, DialogNode> m_dialogues; // int représente la balise de chaque texte

	//std::unordered_map<std::string, size_t> m_dialoguesMap; // Map pour stocker les dialogues chargées, indexées par leur chemin d'accès
	//std::vector<std::string*> m_dialoguesVector; // Vector pour stocker les dialogues chargées, utilisé pour itérer plus rapidement lors de la libération des ressources
	//std::vector<std::string> m_dialoguesIdsVector; // Vector pour stocker les identifiants des dialogues chargées, utilisé pour itérer plus rapidement lors de la libération des ressources

public :

	void ParserDialogueFile(const std::string& p_path);

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

	friend class Application;
};

