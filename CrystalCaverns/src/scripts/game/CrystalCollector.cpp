#include "CrystalCollector.h"
#include "NoiseRingController.h"


void CrystalCollector::Start()
{
	m_Transform = &GetComponent<Transform>();
	// get the crystal map
	FindCrystalMap();
	
	// insert data pertaining to all the different types of crystal
	m_CrystalData.insert({ m_CrystalMap->GetPalette()->GetTileByName("crystal1"), { 5 , 2 , m_CrystalMap->GetPalette()->GetTileByName("brokenCrystal1") } });
	m_CrystalData.insert({ m_CrystalMap->GetPalette()->GetTileByName("crystal2"), { 10, 4 , m_CrystalMap->GetPalette()->GetTileByName("brokenCrystal2") } });
	m_CrystalData.insert({ m_CrystalMap->GetPalette()->GetTileByName("crystal3"), { 20, 6 , m_CrystalMap->GetPalette()->GetTileByName("brokenCrystal3") } });
	m_CrystalData.insert({ m_CrystalMap->GetPalette()->GetTileByName("crystal4"), { 30, 8 , m_CrystalMap->GetPalette()->GetTileByName("brokenCrystal4") } });
	m_CrystalData.insert({ m_CrystalMap->GetPalette()->GetTileByName("crystal5"), { 50, 10, m_CrystalMap->GetPalette()->GetTileByName("brokenCrystal5") } });
	
	// update the score text on the screen
	UpdateText();

	m_PlayerController = &GetNativeScript<PlayerController>();
	m_LightAnimator = &GetNativeScript<PlayerLightAnimator>();

	m_WolfManager = &GetNativeScript<WolfManager>(GetEntitiesWithTag("WolfManager")[0]);
	m_CameraController = &GetNativeScript<CameraController>(GetEntitiesWithTag("MainCamera")[0]);


	// load sounds used for mining
	AudioSystem::LoadSound("mining", "assets/audio/mining.ogg");
	AudioSystem::LoadSound("crack", "assets/audio/crack.ogg");
	AudioSystem::LoadSound("break", "assets/audio/break.ogg");
}

void CrystalCollector::OnSceneLoaded()
{
	// when a new scene is loaded we need to find the new crystal map
	FindCrystalMap();
}

void CrystalCollector::FindCrystalMap()
{
	m_CrystalMap = &GetComponent<Tilemap>(GetEntitiesWithTag("Crystals")[0]);
}

void CrystalCollector::Update(float ts)
{
	// check if we are attempting to mine crystals
	if (Input::IsKeyPressed(sf::Keyboard::Space))
	{
		// are we touching any crystals?
		if (m_CollidingCrystals.size() > 0)
		{
			// find the crystal we should be mining
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
				if (type == NULL_TILE_ID) // something went really wrong, abort mission
					return;
				// subtract one because this counts as one hit
				m_MiningProgress.insert({ currentCrystal, { m_CrystalData[type].Durability - 1, &m_CrystalData[type] } });
			}
			
			// find out how much of the crystal weve mined
			auto& crystal = m_MiningProgress[currentCrystal];

			// if its not broken yet
			if (!crystal.Broken)
			{
				// check if it should crack
				// it cracks when it is mined halfway
				if (crystal.Progress <= 0.5f * crystal.Data->Durability)
				{
					// time to break the crystal
					crystal.Broken = true;
					// overwrite the current tile, replace it with the broken appearing one
					m_CrystalMap->PlaceTile(currentCrystal, crystal.Data->BrokenTile, true);
					m_BrokenCrystal = true;

					// shake the camera and play the sound effect
					m_CameraController->ShakeCamera(0.1f, 3.0f);
					PlayBreakSound("crack", currentCrystal);
				}
			}
			// if the crystal has been mined
			if (crystal.Progress == 0)
			{
				// this crystal has been broken and we can now take the points for it
				// remove it from the tile map
				m_CrystalMap->RemoveTile(currentCrystal);
				ScorePoints(crystal.Data->Value);
				// dont track this tile any longer
				m_MiningProgress.erase(currentCrystal);

				// shake the camera and play the breaking sound
				m_CameraController->ShakeCamera(0.25f, 5.0f);
				PlayBreakSound("break", currentCrystal);
			}

			// get the world coordinates of the centre of the crystal that has just been broken
			// and then create a noise ring game object at that point
			CreateNoiseRing(m_CrystalMap->TileToWorldCoordinates(currentCrystal) + sf::Vector2f{16, 16});
			// make the scene light do a cool blue flash
			m_LightAnimator->BeginAnimate(sf::Color::Blue, 0.4f);
			// play the mining sound
			PlayBreakSound("mining", currentCrystal);

			// collecting crystals will awaken wolves
			// use the centre of the player as the point they are awoken from
			m_WolfManager->AwakenWolves(m_PlayerController->GetCentre());
		}
	}

	// if the display score needs to be brought up to match the actual score
	if (m_DisplayCrystalScore < m_CrystalScore)
	{
		// 50 is the rate at which the display score goes up
		m_DisplayCrystalScore += 50 * ts;
		// update the gui text
		UpdateText();
	}

}

void CrystalCollector::OnTriggerEnter(const Collision& collision)
{
	// did we collide with a crystal?
	if (GetEntityTag(collision.Other) == "Crystals")
	{
		// get the position of the tile in tile coordinates
		sf::Vector2i pos{ m_CrystalMap->WorldToTileCoordinates({ collision.OtherGlobalBounds.left, collision.OtherGlobalBounds.top }) };
		
		// check if this position has already been registered: in which case we have just cracked the cystal
		if (CollidingWithCrystal(pos))
		{
			// we need to update the colliderID in the map
			ColliderID old;
			for (auto& pair : m_CrystalColliders)
			{
				// check if this tile is in the same position as the one we just collided with
				// in which case it is the old tile which is now replaced with a cracked version
				if (pair.second == pos) old = pair.first;
			}
			// erase the old collider ID and add the new one
			m_CrystalColliders.erase(old);
			m_CrystalColliders.insert({ collision.OtherColliderID, pos });
		}
		else
		{
			// otherwise just add in the new collider and its corresponding position
			m_CollidingCrystals.push_back(pos);
			m_CrystalColliders.insert({ collision.OtherColliderID, pos });
		}
	}
}

void CrystalCollector::OnTriggerExit(const std::pair<Entity, ColliderID>& other)
{
	// check if the object was a crystal
	if (GetEntityTag(other.first) == "Crystals")
	{
		// if we were breaking a crystal then no longer flag as such
		if (m_BrokenCrystal)
			m_BrokenCrystal = false;
		else
		{
			// find the coordinates of the crystal we need to remove
			sf::Vector2i toRemove = m_CrystalColliders[other.second];

			// find the index of the crystal that needs to be removed
			int index = 0;
			for (auto& crystal : m_CollidingCrystals)
			{
				if (crystal == toRemove)
				{
					// remove that crystal and break from the loop
					m_CollidingCrystals.erase(m_CollidingCrystals.begin() + index);
					break;
				}
				index++;
			}
			// erase the collider from the map
			m_CrystalColliders.erase(other.second);
		}
	}
}

void CrystalCollector::ScorePoints(int points)
{
	m_CrystalScore += points;
}



void CrystalCollector::UpdateText()
{
	// set the string on the score text
	m_ScoreText->SetString("Score  " + std::to_string(static_cast<int>(m_DisplayCrystalScore)));
}

void CrystalCollector::SetScoreText(Entity scoreText)
{
	m_ScoreText = &GetComponent<GUIText>(scoreText);
}

void CrystalCollector::CreateNoiseRing(const sf::Vector2f& position)
{
	// a noise ring shows the range that wolves will be woken
	Entity newRing = CreateEntity();

	// create the sprite renderer
	// noise rings are rendered using the basic material
	// we do not want them to be affected by lighting
	AddComponent(newRing, Transform{ });
	AddComponent(newRing, SpriteRenderer{
			Texture::Create("assets/textures/noiseRing.png"),
			Material::Create("Basic"),
			3
		});

	// add the noise ring controller script
	auto& script = AddNativeScript<NoiseRingController>(newRing);
	// and set the centre position of the ring
	script.SetCentrePosition(position);
}

void CrystalCollector::PlayBreakSound(const std::string& sound, const sf::Vector2i& currentCrystal)
{
	// we need to move the sound to the position of the current crystal before playing the audio
	AudioSystem::SetPosition(sound, m_CrystalMap->TileToWorldCoordinates(currentCrystal) + sf::Vector2f{ 16, 16 });
	AudioSystem::PlaySound(sound);
}

void CrystalCollector::AssignPlayerData()
{
	// we need to save the score before the scene is deleted so it can be used in the win screen
	int* score = &DataStore::RetrieveData<PlayerData>("playerData")->Score;
	*score = m_CrystalScore;
}

bool CrystalCollector::CollidingWithCrystal(const sf::Vector2i& pos)
{
	// check if the player is currently colliding with a given crystal
	for (auto const& p : m_CollidingCrystals)
	{
		if (p == pos) return true;
	}
	return false;
}
