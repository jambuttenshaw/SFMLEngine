#pragma once

#include <SFMLEngine.h>
#include "WolfManager.h"
#include "CameraController.h"
#include "PlayerData.h"
#include "PlayerLightAnimator.h"


using namespace SFMLEngine;


class MiningNoiseRing;


struct CrystalData
{
	int Value;
	int Durability;
	TileID BrokenTile;
};
struct MiningProgress
{
	int Progress;
	CrystalData* Data;
	bool Broken = false;
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

	std::vector<sf::Vector2i> m_CollidingCrystals;
	bool m_BrokenCrystal = false;

	std::unordered_map<TileID, CrystalData> m_CrystalData;
	std::unordered_map<sf::Vector2i, MiningProgress> m_MiningProgress;

	std::unordered_map<ColliderID, sf::Vector2i> m_CrystalColliders;

	int m_CrystalScore = 0;
	float m_DisplayCrystalScore = 0;
};
