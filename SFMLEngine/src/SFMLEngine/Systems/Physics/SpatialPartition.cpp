#include "SpatialPartition.h"


namespace SFMLEngine {

	SpatialPartition::SpatialPartition(int partitionSize)
		: m_PartitionSize(partitionSize)
	{
		if (m_PartitionSize != 0)
			// why do the divisions every frame when you can just do them once instead
			m_InversePartitionSize = 1.0f / static_cast<float>(m_PartitionSize);
	}


	void SpatialPartition::Insert(ColliderData* newCollider)
	{
		auto [topLeft, bottomRight] = Math::GetCorners(newCollider->ColliderPtr->GetGlobalBounds());
		sf::Vector2i topLeftPartition = WorldToSpacialPartitionCoords(topLeft);
		sf::Vector2i bottomRightPartition = WorldToSpacialPartitionCoords(bottomRight);

		// the collider will ALWAYS exist in the top-left partition
		m_PartitionedColliders.insert(std::make_pair(topLeftPartition, newCollider));

		// it may also exist in other partitions depending on the dimensions of the collider
		bool topRight = bottomRightPartition.x > topLeftPartition.x;
		bool bottomLeft = bottomRightPartition.y > topLeftPartition.y;

		// the partition size will always be selected such that a collider can exist in no more than 4 partitions
		if (topRight)
			m_PartitionedColliders.insert(std::make_pair(sf::Vector2i{ bottomRightPartition.x, topLeftPartition.y }, newCollider));
		if (bottomLeft)
			m_PartitionedColliders.insert(std::make_pair(sf::Vector2i{ topLeftPartition.x, bottomRightPartition.y }, newCollider));
		if (topRight && bottomLeft)
			m_PartitionedColliders.insert(std::make_pair(bottomRightPartition, newCollider));
	}


	void SpatialPartition::Delete(ColliderID id)
	{
		std::vector<std::unordered_multimap<sf::Vector2i, ColliderData*>::iterator> toErase;
		for (auto it = m_PartitionedColliders.begin(); it != m_PartitionedColliders.end(); it++)
		{
			if ((*it).second->ID == id)
			{
				toErase.push_back(it);
			}
		}
		for (auto const& it : toErase) m_PartitionedColliders.erase(it);
	}



	const std::vector<ColliderData*> SpatialPartition::CollidersSharingPartition(const sf::FloatRect& rect)
	{
		auto [topLeft, bottomRight] = Math::GetCorners(rect);

		sf::Vector2i topLeftPartition = WorldToSpacialPartitionCoords(topLeft);
		sf::Vector2i bottomRightPartition = WorldToSpacialPartitionCoords(bottomRight);

		bool topRight = bottomRightPartition.x > topLeftPartition.x;
		bool bottomLeft = bottomRightPartition.y > topLeftPartition.y;

		std::vector<ColliderData*> colliders;
		SearchPartition(topLeftPartition, colliders);
		if (topRight)
			SearchPartition({ bottomRightPartition.x, topLeftPartition.y }, colliders);
		if (bottomLeft)
			SearchPartition({ topLeftPartition.x, bottomRightPartition.y }, colliders);
		if (topRight && bottomLeft)
			SearchPartition(bottomRightPartition, colliders);

		return std::move(colliders);
	}



	sf::Vector2i SpatialPartition::WorldToSpacialPartitionCoords(const sf::Vector2f& position)
	{
		return { static_cast<int>(position.x * m_InversePartitionSize ),
				 static_cast<int>(position.y * m_InversePartitionSize ) };
	}


	void SpatialPartition::SearchPartition(const sf::Vector2i partition, std::vector<ColliderData*>& colliders)
	{
		auto start = m_PartitionedColliders.find(partition);
		if (start == m_PartitionedColliders.end()) return;
		else
		{
			for (auto& itr = start; (itr != m_PartitionedColliders.end()) && (itr->first == partition); itr++)
			{
				// loop through all colliders in this partition and add them to the vector
				colliders.push_back(itr->second);
			}
		}
	}


}
