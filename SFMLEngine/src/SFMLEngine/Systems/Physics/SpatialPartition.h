#pragma once

#include <SFML/Graphics.hpp>

#include "CollisionSystem.h"


namespace SFMLEngine {
	
	class SpatialPartition
	{
	public:
		// constructor that takes in the size of an individual partition
		SpatialPartition(int partitionSize);
		~SpatialPartition() = default;

		void Insert(const ColliderData& newCollider);

		// get a vector of all colliders that share partitions with the rect passed in
		const std::vector<ColliderData*> CollidersSharingPartition(const sf::FloatRect& rect);


	private:
		sf::Vector2i WorldToSpacialPartitionCoords(const sf::Vector2f& pos);

		void SearchPartition(const sf::Vector2i partition, std::vector<ColliderData*>& colliders);
	
	private:
		int m_PartitionSize = 0;
		std::unordered_multimap<sf::Vector2i, ColliderData> m_PartitionedColliders;

	};
	
}
