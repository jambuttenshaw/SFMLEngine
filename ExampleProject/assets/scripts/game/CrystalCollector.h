#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class CrystalCollector : public ScriptableEntity
{
public:
	void Start() override
	{
		m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal1"), 5  });
		m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal4"), 10 });
		m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal2"), 20 });
		m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal5"), 30 });
		m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal3"), 50 });

		UpdateText();
	}

	void Update(Timestep ts) override
	{
		if (Input::IsKeyPressed(sf::Keyboard::Space))
		{
			if (m_CollidingWithCrystal)
			{
				TileID removedType = m_CrystalMap->RemoveTile(m_CollidingCrystalPos);
				m_CrystalScore += m_CrystalValues[removedType];
			}
		}

		if (m_DisplayCrystalScore < m_CrystalScore)
		{
			m_DisplayCrystalScore += 50 * ts;
			UpdateText();
		}

	}

	void OnTriggerEnter(const Collision& collision) override
	{
		if (GetEntityTag(collision.Other) == "Crystals")
		{
			m_CollidingWithCrystal = true;
			m_CollidingCrystalPos = m_CrystalMap->WorldToTileCoordinates({ collision.OtherGlobalBounds.left, collision.OtherGlobalBounds.top });
		}
	}

	void OnTriggerExit(Entity other)
	{
		if (GetEntityTag(other) == "Crystals")
		{
			m_CollidingWithCrystal = false;
		}
	}



	void UpdateText()
	{
		m_ScoreText->SetString("Score  " + std::to_string(static_cast<int>(m_DisplayCrystalScore)));
	}



	void SetCrystalMap(Entity crystals)
	{
		m_CrystalMap = &GetComponent<Tilemap>(crystals);
	}

	void SetScoreText(Entity scoreText)
	{
		m_ScoreText = &GetComponent<GUIText>(scoreText);
	}

private:
	Tilemap* m_CrystalMap = nullptr;

	GUIText* m_ScoreText = nullptr;

	bool m_CollidingWithCrystal = false;
	sf::Vector2i m_CollidingCrystalPos;

	std::unordered_map<TileID, int> m_CrystalValues;

	int m_CrystalScore = 0;
	float m_DisplayCrystalScore = 0;
};
