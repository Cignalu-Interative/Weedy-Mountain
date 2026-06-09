#pragma once
#include "2DPendentLib/Text.h"

class Dialogue
{
private : 

	std::unordered_map<int, DialogNode> m_dialogues; // int représente la balise de chaque texte
	std::vector<Text*> m_texts;

	Text* m_currentText = nullptr;

public : 

	Dialogue() = default;
	~Dialogue();

	void SetPreviousDialogue();
	void SetNextDialogue();

	void JumpToBalise(int p_balise);

};

