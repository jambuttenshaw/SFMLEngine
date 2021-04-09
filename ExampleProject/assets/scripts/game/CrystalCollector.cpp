#include "CrystalCollector.h"
#include "NoiseRingController.h"


void CrystalCollector::Start()
{
	m_Transform = &GetComponent<Transform>();
	FindCrystalMap();
	
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal1"), { 5 , 2 , m_CrystalMap->PalettePtr->GetTileByName("brokenCrystal1") } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal2"), { 10, 4 , m_CrystalMap->PalettePtr->GetTileByName("brokenCrystal2") } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal3"), { 20, 6 , m_CrystalMap->PalettePtr->GetTileByName("brokenCrystal3") } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal4"), { 30, 8 , m_CrystalMap->PalettePtr->GetTileByName("brokenCrystal4") } });
	m_CrystalData.insert({ m_CrystalMap->PalettePtr->GetTileByName("crystal5"), { 50, 10, m_CrystalMap->PalettePtr->GetTileByName("brokenCrystal5") } });
	
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
	if (Input::IsKeyPressed(sf::Keyboard::J))
	{
		sf::Vector2f pos = m_CrystalMap->TileToWorldCoordinates(m_CrystalMap->WorldToTileCoordinates(m_Transform->Position));
		LOG_TRACE("Player head is in tile at: {0}, {1}", pos.x, pos.y);
	}
	
	if (Input::IsKeyPressed(sf::Keyboard::Space))
	{
		if (m_CollidingCrystals.size() > 0)
		{
			sf::Vector2i currentCrystal = m_CollidingCrystals[m_CollidingCrystals.size() - 1];
			// time to make progress in mining a crystal!
			// check if weve already begun hacking away at this crystal
			if (m_MiningProgress.find(currentCrystal) != m_MiningProgress.end())
			{
				// weve already begun mining this crystal: make progress
				m_MiningProgress[currentCrystal].Progress -= 1;
				
			}
			else
			{
				// this crystal hasn't been touched before, so register as it being mined
				TileID type = m_CrystalMap->GetTileAtLocation(currentCrystal);
				// subtract one because this counts as one hit
				m_MiningProgress.insert({ currentCrystal, { m_CrystalData[type].Durability - 1, &m_CrystalData[type] } });
			}
			

			auto& crystal = m_MiningProgress[currentCrystal];

			if (!crystal.Broken)
			{
				if (crystal.Progress <= 0.5f * crystal.Data->Durability)
				{
					// time to break the crystal
					crystal.Broken = true;
					// overwrite the current tile, replace it with the broken appearing one
					m_CrystalMap->PlaceTile(currentCrystal, crystal.Data->BrokenTile, true);
					m_BrokenCrystal = true;
				}
			}
			// if the crystal has been mined
			if (crystal.Progress == 0)
			{
				// this crystal has been broken and we can now take the points for it
				m_CrystalMap->RemoveTile(currentCrystal);
				m_CrystalScore += crystal.Data->Value;
				m_MiningProgress.erase(currentCrystal);
			}

			CreateNoiseRing(m_CrystalMap->TileToWorldCoordinates(currentCrystal) + sf::Vector2f{16, 16});

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
		sf::Vector2i pos{ m_CrystalMap->WorldToTileCoordinates({ collision.OtherGlobalBounds.left, collision.OtherGlobalBounds.top }) };
		
		// check if this position has already been registered: in which case we have just broken the cystal
		if (CollidingWithCrystal(pos))
		{
			// we need to update the colliderID in the map
			ColliderID old;
			for (auto& pair : m_CrystalColliders)
			{
				if (pair.second == pos) old = pair.first;
			}
			m_CrystalColliders.erase(old);
			m_CrystalColliders.insert({ collision.OtherColliderID, pos });
		}
		else
		{
			m_CollidingCrystals.push_back(pos);
			m_CrystalColliders.insert({ collision.OtherColliderID, pos });
		}
	}
}

void CrystalCollector::OnTriggerExit(const std::pair<Entity, ColliderID>& other)
{
	if (GetEntityTag(other.first) == "Crystals")
	{
		if (m_BrokenCrystal)
			m_BrokenCrystal = false;
		else
		{
			sf::Vector2i toRemove = m_CrystalColliders[other.second];
			int index = 0;
			for (auto& crystal : m_CollidingCrystals)
			{
				if (crystal == toRemove)
				{
					m_CollidingCrystals.erase(m_CollidingCrystals.begin() + index);
					break;
				}
				index++;
			}
			m_CrystalColliders.erase(other.second);
		}
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

bool CrystalCollector::CollidingWithCrystal(const sf::Vector2i& pos)
{
	for (auto const& p : m_CollidingCrystals)
	{
		if (p == pos) return true;
	}
	return false;
}
