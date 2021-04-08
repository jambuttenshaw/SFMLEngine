#pragma once

#include <SFMLEngine.h>
#include "WolfManager.h"


using namespace SFMLEngine;


class MiningNoiseRing;


struct CrystalData
{
	int Value;
	int Durability;
};


class CrystalCollector : public ScriptableEntity
{
public:
	void Start() override;
	void OnSceneLoaded() override;
	void FindCrystalMap();

	void Update(Timestep ts) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(Entity other) override;

	void UpdateText();

	void SetScoreText(Entity scoreText);
	void SetScenePtr(Scene* scene) { m_Scene = scene; }

	void CreateMiningRing(const sf::Vector2f& position);
private:
	Scene* m_Scene = nullptr;

	Transform* m_Transform = nullptr;
	Tilemap* m_CrystalMap = nullptr;

	GUIText* m_ScoreText = nullptr;

	WolfManager* m_WolfManager = nullptr;

	bool m_CollidingWithCrystal = false;
	sf::Vector2i m_CollidingCrystalPos;

	std::unordered_map<TileID, CrystalData> m_CrystalData;
	std::unordered_map<sf::Vector2i, int> m_MiningProgress;

	int m_CrystalScore = 0;
	float m_DisplayCrystalScore = 0;
};
