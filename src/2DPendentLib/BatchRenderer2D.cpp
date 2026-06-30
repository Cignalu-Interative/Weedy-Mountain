#include "pch.h"
#include "BatchRenderer2D.h"
#include "Render.h"

BatchRenderer2D* BatchRenderer2D::Get()
{
	static BatchRenderer2D instance;
	return &instance;
}

BatchRenderer2D::BatchRenderer2D()
{
	m_commands.reserve(10000); // Réserve de l'espace pour 1000 commandes de rendu
	m_batches.reserve(100); // Réserve de l'espace pour 100 batches
}

BatchRenderer2D::~BatchRenderer2D()
{

}

const std::vector<Data>& BatchRenderer2D::Flush()
{
	for (auto& batch : m_batches)
	{
		batch.s_vertices.clear(); // garde la capacité
		batch.s_indices.clear();
		batch.s_texture = nullptr;
	}

	if (m_commands.empty())
	{
		m_commands.clear();
		return m_batches;
	}

	//m_vertexBuffer.clear(); // Garde la capacité, remet à zéro la taille
	//m_indicesBuffer.clear();

	//m_vertexBuffer.reserve(m_commands.size() * 4);
	//m_indicesBuffer.reserve(m_commands.size() * 6);

	std::unordered_map<SDL_Texture*, size_t> batchIndexByTexture;
	size_t batchCount = 0;
    for (const auto& command : m_commands)
    {
        std::visit([&](auto&& p_command)
            {
                SDL_Texture* texture = p_command.s_texture;

                size_t batchIndex;
                auto it = batchIndexByTexture.find(texture);
                if (it == batchIndexByTexture.end())
                {
                    batchIndex = batchCount++;
                    batchIndexByTexture[texture] = batchIndex;

                    if (batchIndex >= m_batches.size())
                        m_batches.push_back(Data{}); // ne grandit que si vraiment nécessaire

                    m_batches[batchIndex].s_texture = texture;
                }
                else
                {
                    batchIndex = it->second;
                }

                Data& batch = m_batches[batchIndex];
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

	// Redimensionne le vecteur m_batches pour ne conserver que les batches utilisés
    if (batchCount < m_batches.size())
        m_batches.resize(batchCount);

    m_commands.clear();
    return m_batches;
}