#pragma once
#include "2DPendentLib/Text.h"

class Dialogue
{

public :

	struct Choice
	{
		std::string s_label;  // "Oui", "NON"
		int s_target;         // @3, @4
	};

private : 

	struct DialogNode
	{
		int s_id;
		std::string s_speaker;
		std::string s_text;
		std::vector<Choice> s_choices;  // vide = pas de question

		DialogNode()
		{
			s_choices.reserve(4); // On reserve 4 choix par defaut pour eviter les reallocations
		}
	};

	std::unordered_map<int, DialogNode> m_dialogues; // int représente la balise de chaque texte

public :
 
	Dialogue();

	~Dialogue();

	bool ParserDialogueFile(const std::string& p_path);

	void JumpToBalise(int p_id);

	std::string* GetSpeakerById(int p_id);
	std::string* GetTextById (int p_id);

	std::vector<Choice> GetChoicesById(int p_id);

};

