#include "CrystalCollector.h"

#include "WolfController.h"


void CrystalCollector::Start()
{
	m_Transform = &GetComponent<Transform>();


	m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal1"), 5 });
	m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal4"), 10 });
	m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal2"), 20 });
	m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal5"), 30 });
	m_CrystalValues.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal3"), 50 });

	UpdateText();
}

void CrystalCollector::Update(Timestep ts)
{
	if (Input::IsKeyPressed(sf::Keyboard::Space))
	{
		if (m_CollidingWithCrystal)
		{
			TileID removedType = m_CrystalMap->RemoveTile(m_CollidingCrystalPos);
			m_CrystalScore += m_CrystalValues[removedType];

			// collecting crystals will awaken wolves
			AwakenWolves();
		}
	}

	if (m_DisplayCrystalScore < m_CrystalScore)
	{
		m_DisplayCrystalScore += 50 * ts;
		UpdateText();
	}

}

void CrystalCollector::OnTriggerEnter(const Collision& collision)
{
	if (GetEntityTag(collision.Other) == "Crystals")
	{
		m_CollidingWithCrystal = true;
		m_CollidingCrystalPos = m_CrystalMap->WorldToTileCoordinates({ collision.OtherGlobalBounds.left, collision.OtherGlobalBounds.top });
	}
}

void CrystalCollector::OnTriggerExit(Entity other)
{
	if (GetEntityTag(other) == "Crystals")
	{
		m_CollidingWithCrystal = false;
	}
}



void CrystalCollector::UpdateText()
{
	m_ScoreText->SetString("Score  " + std::to_string(static_cast<int>(m_DisplayCrystalScore)));
}



void CrystalCollector::SetCrystalMap(Entity crystals)
{
	m_CrystalMap = &GetComponent<Tilemap>(crystals);
}

void CrystalCollector::SetScoreText(Entity scoreText)
{
	m_ScoreText = &GetComponent<GUIText>(scoreText);
}

void CrystalCollector::AwakenWolves()
{
	std::vector<Entity> wolves = GetEntitiesWithTag("Wolf");
	for (auto const& wolf : wolves)
	{
		auto& transform = GetComponent<Transform>(wolf);
		if (Math::SquareMagnitude(transform.Position - m_Transform->Position) < m_WolfTriggerRadius * m_WolfTriggerRadius)
		{
			auto& wolfController = GetNativeScript<WolfController>(wolf);
			wolfController.Wake();
		}
	}
}
