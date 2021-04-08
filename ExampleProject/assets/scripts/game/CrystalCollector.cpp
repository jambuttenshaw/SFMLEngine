#include "CrystalCollector.h"
#include "NoiseRingController.h"


void CrystalCollector::Start()
{
	m_Transform = &GetComponent<Transform>();
	FindCrystalMap();
	
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal1"), { 5 , 2  } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal2"), { 10, 4  } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal3"), { 20, 6  } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal4"), { 30, 8  } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal5"), { 50, 10 } });
	
	UpdateText();


	m_WolfManager = &GetNativeScript<WolfManager>(GetEntitiesWithTag("WolfManager")[0]);
}

void CrystalCollector::OnSceneLoaded()
{
	FindCrystalMap();
}

void CrystalCollector::FindCrystalMap()
{
	m_CrystalMap = &GetComponent<Tilemap>(GetEntitiesWithTag("Crystals")[0]);
}

void CrystalCollector::Update(Timestep ts)
{
	if (Input::IsKeyPressed(sf::Keyboard::Space))
	{
		if (m_CollidingWithCrystal)
		{
			// time to make progress in mining a crystal!
			// check if weve already begun hacking away at this crystal
			if (m_MiningProgress.find(m_CollidingCrystalPos) != m_MiningProgress.end())
			{
				// weve already begun mining this crystal: make progress
				m_MiningProgress[m_CollidingCrystalPos] -= 1;
			}
			else
			{
				// this crystal hasn't been touched before, so register as it being mined
				TileID type = m_CrystalMap->GetTileAtLocation(m_CollidingCrystalPos);
				// subtract one because this counts as one hit
				m_MiningProgress.insert({ m_CollidingCrystalPos, m_CrystalData[type].Durability - 1 });
			}

			// if the crystal has been mined
			if (m_MiningProgress[m_CollidingCrystalPos] == 0)
			{
				// this crystal has been broken and we can now take the points for it
				TileID removedType = m_CrystalMap->RemoveTile(m_CollidingCrystalPos);
				m_CrystalScore += m_CrystalData[removedType].Value;
				m_MiningProgress.erase(m_CollidingCrystalPos);
			}

			CreateNoiseRing(m_CrystalMap->TileToWorldCoordinates(m_CollidingCrystalPos) + sf::Vector2f{16, 16});

			// collecting crystals will awaken wolves
			m_WolfManager->AwakenWolves(m_Transform->Position);
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

void CrystalCollector::SetScoreText(Entity scoreText)
{
	m_ScoreText = &GetComponent<GUIText>(scoreText);
}

void CrystalCollector::CreateNoiseRing(const sf::Vector2f& position)
{
	Entity newRing = CreateEntity();

	AddComponent(newRing, Transform{ });
	AddComponent(newRing, SpriteRenderer{
			Texture::Create("assets/textures/noiseRing.png"),
			Material::Create("Basic"),
			3
		});

	auto& script = AddNativeScript<NoiseRingController>(newRing);
	script.SetCentrePosition(position);
}
