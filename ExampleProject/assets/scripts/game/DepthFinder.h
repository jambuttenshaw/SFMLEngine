#pragma once

#include <SFMLEngine.h>
using namespace SFMLEngine;


class DepthFinder : public ScriptableEntity
{
public:

	void Start() override
	{
		m_PlayerTransform = &GetComponent<Transform>(GetEntitiesWithTag("Player")[0]);


		// create the depth marker
		m_DepthMarker = CreateEntity();

		GUITransform t{ { 0.957f, m_GUIMinY }, GUIElementType::Image };
		t.SetHorizontalAnchor(GUITransform::Anchor::Right);
		t.SetVerticalAnchor(GUITransform::Anchor::Middle);
		AddComponent(m_DepthMarker, t);
		AddComponent(m_DepthMarker, GUIImage{ Texture::Create("assets/textures/depthMarker.png"), 0 });

		m_DepthMarkerTransform = &GetComponent<GUITransform>(m_DepthMarker);



		m_BestDepthMarker = CreateEntity();
		
		GUITransform t2{ { 0.957f, m_GUIMinY }, GUIElementType::Image };
		t2.SetHorizontalAnchor(GUITransform::Anchor::Right);
		t2.SetVerticalAnchor(GUITransform::Anchor::Middle);
		AddComponent(m_BestDepthMarker, t2);
		AddComponent(m_BestDepthMarker, GUIImage{ Texture::Create("assets/textures/bestDepthMarker.png"), 1 });

		// get the size of the depth meter so we can correctly position the markers
		m_BestDepthMarkerTransform = &GetComponent<GUITransform>(m_BestDepthMarker);
	}


	void OnSceneLoaded() override
	{
		m_BestDepth = std::numeric_limits<float>::min();

		m_GroundTilemap = &GetComponent<Tilemap>(GetEntitiesWithTag("Ground")[0]);

		// remember y-axis is flipped
		m_LowestPoint = std::numeric_limits<float>::min();
		m_HighestPoint = std::numeric_limits<float>::max();
		for (auto const& tile : m_GroundTilemap->GetTiles())
		{
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
		float y = Math::Lerp(m_GUIMinY, m_GUIMaxY, (m_PlayerTransform->GetPosition().y - m_HighestPoint) / (m_LowestPoint - m_HighestPoint));
		m_BestDepth = std::max(m_BestDepth, y);

		m_DepthMarkerTransform->SetPosition({ 0.957f, y });
		m_BestDepthMarkerTransform->SetPosition({ 0.957f, m_BestDepth });
	}

private:
	Tilemap* m_GroundTilemap = nullptr;

	Transform* m_PlayerTransform = nullptr;

	float m_HighestPoint = 0;
	float m_LowestPoint = 0;


	float m_GUIMinY = 0.185f;
	float m_GUIMaxY = 0.819f;


	Entity m_DepthMarker = INVALID_ENTITY_ID;
	GUITransform* m_DepthMarkerTransform = nullptr;
	
	Entity m_BestDepthMarker = INVALID_ENTITY_ID;
	GUITransform* m_BestDepthMarkerTransform = nullptr;
	float m_BestDepth = std::numeric_limits<float>::min();
};

