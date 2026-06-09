#pragma once
class DialogueManager
{
private :
	std::unordered_map<std::string, size_t> m_dialoguesMap; // Map pour stocker les dialogues chargées, indexées par leur chemin d'accès


public :

	//Singleton de l'FontManager
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

