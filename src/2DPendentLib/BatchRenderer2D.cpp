#include "pch.h"
#include "BatchRenderer2D.h"
#include "Render.h"
#include "AssetManager.h"

BatchRenderer2D* BatchRenderer2D::Get()
{
	static BatchRenderer2D instance;
	return &instance;
}

BatchRenderer2D::BatchRenderer2D()
{
	m_commands.reserve(10000); // R�serve de l'espace pour 1000 commandes de rendu
	m_batches.reserve(100); // R�serve de l'espace pour 100 batches
}

BatchRenderer2D::~BatchRenderer2D()
{

}

const std::vector<Data>& BatchRenderer2D::Flush()
{
	for (auto& batch : m_batches)
	{
		batch.s_vertices.clear();
		batch.s_indices.clear();
		batch.s_texture = nullptr;
	}

	if (m_commands.empty())
		return m_batches;

	size_t batchCount = 0;
	SDL_Texture* lastTexture = nullptr;
	bool hasBatch = false;

	for (const auto& command : m_commands)
	{
		std::visit([&](auto&& p_command)
			{
				SDL_Texture* texture = p_command.s_texture;

				// Nouveau batch seulement si la texture change (ou 1er passage) :
				// garde l'ordre de dessin (donc le draw layer) intact.
				if (!hasBatch || texture != lastTexture)
				{
					if (batchCount >= m_batches.size())
					{
						m_batches.push_back(Data{});
					}

					m_batches[batchCount].s_texture = texture;
					batchCount++;
					lastTexture = texture;
					hasBatch = true;
				}

				Data& batch = m_batches[batchCount - 1];
				int indexOffset = static_cast<int>(batch.s_vertices.size());

				this->AddQuadToBuffer(batch.s_vertices, p_command);

				batch.s_indices.push_back(indexOffset + 0);
				batch.s_indices.push_back(indexOffset + 1);
				batch.s_indices.push_back(indexOffset + 2);
				batch.s_indices.push_back(indexOffset + 1);
				batch.s_indices.push_back(indexOffset + 2);
				batch.s_indices.push_back(indexOffset + 3);

			}, command);
	}

	if (batchCount < m_batches.size())
		m_batches.resize(batchCount);

	m_commands.clear();
	return m_batches;
}