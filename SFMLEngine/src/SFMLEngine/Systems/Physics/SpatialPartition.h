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

		void Insert(ColliderData newCollider);
		void Erase(Entity entity);

		// get a vector of all colliders that share partitions with the rect passed in
		const std::vector<ColliderData*> CollidersSharingPartition(const sf::FloatRect& rect);

	private:
		void Delete(ColliderID id);

		sf::Vector2i WorldToSpacialPartitionCoords(const sf::Vector2f& pos);

		void SearchPartition(const sf::Vector2i partition, std::vector<ColliderData*>& colliders);
	
	private:
		int m_PartitionSize = 0;
		float m_InversePartitionSize = 0;

		std::vector<ColliderData> m_Colliders;
		std::unordered_multimap<sf::Vector2i, size_t> m_PartitionedColliders;

	};
	
}
