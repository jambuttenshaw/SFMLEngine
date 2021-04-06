#pragma once

#include <SFMLEngine.h>

using namespace SFMLEngine;

class CrystalCollector : public ScriptableEntity
{
public:
	void Start() override;

	void Update(Timestep ts) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(Entity other) override;

	void UpdateText();

	void SetCrystalMap(Entity crystals);
	void SetScoreText(Entity scoreText);

	void AwakenWolves();

private:
	Transform* m_Transform = nullptr;
	Tilemap* m_CrystalMap = nullptr;

	GUIText* m_ScoreText = nullptr;

	bool m_CollidingWithCrystal = false;
	sf::Vector2i m_CollidingCrystalPos;

	std::unordered_map<TileID, int> m_CrystalValues;

	int m_CrystalScore = 0;
	float m_DisplayCrystalScore = 0;


	float m_WolfTriggerRadius = 200.0f;
};
