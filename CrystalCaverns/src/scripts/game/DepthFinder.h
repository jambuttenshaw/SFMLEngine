#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;

#include "PlayerData.h"


/*
The depth finder manages the bar at the rhs of the screen
that displays how deep into the cave the player has travelled
*/
class DepthFinder : public ScriptableEntity
{
public:

	void Start() override
	{
		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);


		// create the depth marker
		m_DepthMarker = CreateEntity();

		// place it at the top of the bar to start
		GUITransform t{ { 0.957f, m_GUIMinY }, GUIElementType::Image };
		// set the anchor so that the point of the arrow is where we specify the position
		t.SetHorizontalAnchor(GUITransform::Anchor::Right);
		t.SetVerticalAnchor(GUITransform::Anchor::Middle);
		AddComponent(m_DepthMarker, t);
		AddComponent(m_DepthMarker, GUIImage{ Texture::Create("assets/textures/depthMarker.png"), 1 });

		// keep a pointer to its transform so we can move it
		m_DepthMarkerTransform = &GetComponent<GUITransform>(m_DepthMarker);


		// repeat the same process for the "best depth" marker
		// except use a different texture
		m_BestDepthMarker = CreateEntity();
		
		GUITransform t2{ { 0.957f, m_GUIMinY }, GUIElementType::Image };
		t2.SetHorizontalAnchor(GUITransform::Anchor::Right);
		t2.SetVerticalAnchor(GUITransform::Anchor::Middle);
		AddComponent(m_BestDepthMarker, t2);
		AddComponent(m_BestDepthMarker, GUIImage{ Texture::Create("assets/textures/bestDepthMarker.png"), 0 });

		m_BestDepthMarkerTransform = &GetComponent<GUITransform>(m_BestDepthMarker);
	}


	void OnSceneLoaded() override
	{
		// when a scene loads, we need to work out the height of the level

		// reset the best depth to the lowest possible value
		m_BestDepth = std::numeric_limits<float>::min();

		// find the ground entity
		m_GroundTilemap = &GetComponent<Tilemap>(GetEntitiesWithTag("Ground")[0]);

		// remember y-axis is flipped
		// so the lowest point has a higher y-value than the highest point
		m_LowestPoint = std::numeric_limits<float>::min();
		m_HighestPoint = std::numeric_limits<float>::max();
		// loop through all the tiles in the ground
		for (auto const& tile : m_GroundTilemap->GetTiles())
		{
			// and find the highest and lowest tiles
			m_LowestPoint = std::max(m_GroundTilemap->GetTileSize().y * tile.Position.y, m_LowestPoint);
			m_HighestPoint = std::min(m_GroundTilemap->GetTileSize().y * tile.Position.y, m_HighestPoint);
		}
	}

	float GetHighestPoint()
	{
		return m_HighestPoint;
	}

	float GetLowestPoint()
	{
		return m_LowestPoint;
	}


	void Update(float ts) override
	{
		// work out the current percentage between the lowest and highest point the player is at
		// as this is between 0 and 1, I can just LERP between the top and bottom points of the depth bar to find the new position for the marker
		float y = Math::Lerp(m_GUIMinY, m_GUIMaxY, (m_PlayerTransform->GetPosition().y - m_HighestPoint) / (m_LowestPoint - m_HighestPoint));
		// if the y position is the biggest yet we want to assign that into the best depth
		m_BestDepth = std::max(m_BestDepth, y);

		// position both of the markers
		m_DepthMarkerTransform->SetPosition({ 0.957f, y });
		m_BestDepthMarkerTransform->SetPosition({ 0.957f, m_BestDepth });
	}

	void AssignPlayerData()
	{
		// this is called before the game goes to the win screen
		// as the best depth is used for scoring purposes, we need to pass that into the data store so it can be accessed in another scene
		float* depth = &DataStore::RetrieveData<PlayerData>("playerData")->DepthPercent;
		// depth should be in the range 0,1, so we do a quick conversion
		*depth = (m_BestDepth - m_GUIMinY) / (m_GUIMaxY - m_GUIMinY);
	}

private:
	Tilemap* m_GroundTilemap = nullptr;

	Transform* m_PlayerTransform = nullptr;

	float m_HighestPoint = 0;
	float m_LowestPoint = 0;

	// pre-determined points 
	// these are the top and bottom of the depth bar and they will never change
	const float m_GUIMinY = 0.185f;
	const float m_GUIMaxY = 0.819f;


	Entity m_DepthMarker = INVALID_ENTITY_ID;
	GUITransform* m_DepthMarkerTransform = nullptr;
	
	Entity m_BestDepthMarker = INVALID_ENTITY_ID;
	GUITransform* m_BestDepthMarkerTransform = nullptr;
	float m_BestDepth = std::numeric_limits<float>::min();
};

