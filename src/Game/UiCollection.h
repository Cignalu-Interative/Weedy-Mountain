// UiCollection.h
#pragma once
#include "Entity.h"
#include <vector>
#include <string>

class ItemBlock;
class Button;

class UiCollection : public Entity
{
private:
	std::vector<ItemBlock*> m_blocks;
	std::vector<std::string> m_allItemIDs;

	Button* m_prevPageButton = nullptr;
	Button* m_nextPageButton = nullptr;
	Button* m_backButton = nullptr;

	Text* m_itemPageName = nullptr;
	Text* m_itemPageDescription = nullptr;
	Text* m_itemPageRecipies = nullptr;

	int m_currentPage = 0;
	int m_blocksPerPage = 16;
	int m_columns = 8;

	bool m_isItemSelected = false;
	bool m_isOpen = false;

	void OnInitialize() override;
	void OnUpdate() override;

	void BuildGrid();
	void RefreshPage();
	void ClearPage();
	void PreviousPage();
	void NextPage();

	void OnItemClick(ItemBlock* block);

public:
	void SetIsVisible(bool p_isVisible);
	void Open();
	void Close();
	bool GetIsOpen() { return m_isOpen; }
};