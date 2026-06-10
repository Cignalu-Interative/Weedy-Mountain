#include "pch.h"
#include "Dialogue.h"
#include <regex>

Dialogue::~Dialogue()
{
}

bool Dialogue::ParserDialogueFile(const std::string& p_path)
{
	std::ifstream file(p_path);
	if (!file.is_open())
	{
		std::cout << "Engine/DialogueManager.cpp/ParserDialogueFile/failed to open file: %s" << p_path.c_str() << std::endl;
		return false;
	}

	std::string line; // pour iterer dans chaque ligne du fichier

	std::regex sentence(R"REGEX(@(\d+)\s+"(.*?)\s-\s(.*?)")REGEX"); // permet de "filtrer" les informations que l on veut recupere 
	std::regex conditional(R"REGEX(\s+\[([^=]+)"\s*=\s*@(\d+)\s*\])REGEX");
	std::smatch match; // permet de stocker les valeurs demander par le regex

	while (std::getline(file, line))
	{
		DialogNode node;

		if (std::regex_search(line, match, sentence))
		{
			node.s_id = std::stoi(match[1]);
			node.s_speaker = match[2];
			node.s_text = match[3];

			for (std::sregex_iterator it(line.begin(), line.end(), conditional);
				it != std::sregex_iterator();
				++it)
			{
				match = *it;

				Choice choice;

				choice.s_label = match[1];
				choice.s_target = std::stoi(match[2]);

				node.s_choices.push_back(choice);
			}
		}
		m_dialogues[node.s_id] = node;
	}
}

void Dialogue::SetPreviousDialogue()
{
}

void Dialogue::SetNextDialogue()
{
}

void Dialogue::JumpToBalise(int p_balise)
{
}
