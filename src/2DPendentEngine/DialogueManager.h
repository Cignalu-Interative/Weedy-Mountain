#pragma once
#include "2DPendentLib/Text.h"

class Dialogue;
class DialogueManager
{

private :

	SDL_Renderer* m_renderer = nullptr;

	std::unordered_map<std::string, size_t> m_dialoguesMap; // Map pour stocker les dialogues chargées, indexées par leur chemin d'accès  string == nom du fichier size_t == emplacement du vector
	std::vector<Dialogue*> m_dialoguesVector; // Vector pour stocker les dialogues chargées, utilisé pour itérer plus rapidement lors de la libération des ressources

	Text* m_currentDialogue = nullptr;

	Text* GetCurrentDialogue() { return m_currentDialogue; }
public :

	void SetRenderer(SDL_Renderer* p_renderer) { m_renderer = p_renderer; }
	SDL_Renderer* GetRenderer() { return m_renderer; }
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

	void SetDialogueToDraw(const std::string& p_nameFile, int p_id);
	// Obtient une texture à partir de son identifiant
	Text* GetDialogueById(const std::string& p_nameFile, int p_id);

	void DestroyDialogueById(const std::string& p_id);

	friend class Application;
};

