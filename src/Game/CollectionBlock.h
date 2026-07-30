#pragma once
#include "Button.h"
#include <string>

class ItemBlock : public Button
{
private:
	std::string m_itemID;
	bool m_unlocked = false;

	int m_index = -1;
	void OnInitialize() override;

public:
	// Affiche l'item si debloque, sinon "???" + icone inconnue
	void SetItem(const std::string& itemID);
	const std::string& GetItemID() const { return m_itemID; }
	bool IsUnlocked() const { return m_unlocked; }
	void SetIndex(int p_index);
	int GetIndex() { return m_index; }

	void SetVisible(bool p_visible);
};