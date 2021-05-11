#pragma once

#include <SFMLEngine.h>
#include "WolfManager.h"
#include "CameraController.h"
#include "PlayerData.h"
#include "PlayerLightAnimator.h"


using namespace SFMLEngine;


class MiningNoiseRing;

// data pertaining to a certain type of crystal
struct CrystalData
{
	int Value; // how many points are gained from mining the crystal
	int Durability; // how many hits it takes to mine a crystal
	TileID BrokenTile; // the tile ID of this crystals respective broken texture
};
// information on crystals that are currently being mined
struct MiningProgress
{
	int Progress = 0; // how much mining has been done on it
	CrystalData* Data = nullptr; // the data about the type of crystal that is being mined
	bool Broken = false; // if it has been cracked or not yet
};


class CrystalCollector : public ScriptableEntity
{
public:
	void Start() override;
	void OnSceneLoaded() override;
	void FindCrystalMap();

	void Update(float ts) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const std::pair<Entity, ColliderID>& other) override;

	void ScorePoints(int points);

	void UpdateText();

	void SetScoreText(Entity scoreText);
	void SetScenePtr(Scene* scene) { m_Scene = scene; }

	void CreateNoiseRing(const sf::Vector2f& position);

	void PlayBreakSound(const std::string& sound, const sf::Vector2i& currentCrystal);

	void AssignPlayerData();

private:

	bool CollidingWithCrystal(const sf::Vector2i& pos);

private:
	Scene* m_Scene = nullptr;

	Transform* m_Transform = nullptr;
	Tilemap* m_CrystalMap = nullptr;

	GUIText* m_ScoreText = nullptr;

	WolfManager* m_WolfManager = nullptr;

	PlayerController* m_PlayerController = nullptr;
	CameraController* m_CameraController = nullptr;
	PlayerLightAnimator* m_LightAnimator = nullptr;

	// all of the crystals that we are currently in contact with
	std::vector<sf::Vector2i> m_CollidingCrystals;
	bool m_BrokenCrystal = false;

	// information about each type of crystal in the world
	std::unordered_map<TileID, CrystalData> m_CrystalData;
	
	// data about crystals that we are currently mining
	std::unordered_map<sf::Vector2i, MiningProgress> m_MiningProgress;
	// information about where each collider is located
	// so when the crystal cracks, and its collider is swapped out,
	// we can maintain our knowledge of what crystal is where by updating the collider ID
	std::unordered_map<ColliderID, sf::Vector2i> m_CrystalColliders;

	int m_CrystalScore = 0;
	float m_DisplayCrystalScore = 0;
};
