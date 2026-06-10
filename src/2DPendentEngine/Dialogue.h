#pragma once
#include "2DPendentLib/Text.h"

class Dialogue
{
private : 

	struct Choice
	{
		std::string s_label;  // "Oui", "NON"
		int s_target;         // @3, @4
	};

	struct DialogNode
	{
		int s_id;
		std::string s_speaker;
		Text s_text;
		std::vector<Choice*> s_choices;  // vide = pas de question
	};


	std::unordered_map<int, DialogNode> m_dialogues; // int représente la balise de chaque texte

public : 

	Dialogue() = default;
	~Dialogue();

	bool ParserDialogueFile(const std::string& p_path);

	void SetPreviousDialogue();
	void SetNextDialogue();

	void JumpToBalise(int p_balise);

	std::string* GetSpeakerById(int p_id);
	Text* GetTextById (int p_id);

};

