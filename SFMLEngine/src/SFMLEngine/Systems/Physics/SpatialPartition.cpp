#include "SpatialPartition.h"


namespace SFMLEngine {

	SpatialPartition::SpatialPartition(int partitionSize)
		: m_PartitionSize(partitionSize)
	{
		if (m_PartitionSize != 0)
			// why do the divisions every frame when you can just do them once instead
			m_InversePartitionSize = 1.0f / static_cast<float>(m_PartitionSize);
	}


	void SpatialPartition::Insert(ColliderData newCollider)
	{
		// always insert a collider at the back of the vector
		size_t newIndex = m_Colliders.size();
		m_Colliders.push_back(newCollider);

		// then the multimap will map from a partition to the index of the collider

		auto [topLeft, bottomRight] = Math::GetCorners(newCollider.ColliderPtr->GetGlobalBounds());
		sf::Vector2i topLeftPartition = WorldToSpacialPartitionCoords(topLeft);
		sf::Vector2i bottomRightPartition = WorldToSpacialPartitionCoords(bottomRight);

		// the collider will ALWAYS exist in the top-left partition
		m_PartitionedColliders.insert(std::make_pair(topLeftPartition, newIndex));

		// it may also exist in other partitions depending on the dimensions of the collider
		bool topRight = bottomRightPartition.x > topLeftPartition.x;
		bool bottomLeft = bottomRightPartition.y > topLeftPartition.y;

		// the partition size will always be selected such that a collider can exist in no more than 4 partitions
		if (topRight)
			m_PartitionedColliders.insert(std::make_pair(sf::Vector2i{ bottomRightPartition.x, topLeftPartition.y }, newIndex));
		if (bottomLeft)
			m_PartitionedColliders.insert(std::make_pair(sf::Vector2i{ topLeftPartition.x, bottomRightPartition.y }, newIndex));
		if (topRight && bottomLeft)
			m_PartitionedColliders.insert(std::make_pair(bottomRightPartition, newIndex));
	}


	void SpatialPartition::Erase(Entity entity)
	{
		// remove all collider data associated with entity from the spacial partition
		// the steps are as follows
		// start at the end of the vector of colliders in the vector
		//  - once a collider is found belonging to this entity, delete all of its entries in the multimap
		//  - if it belongs to this entity, find all of their entries in the partitioning map and flag for removal

		for (int i = static_cast<int>(m_Colliders.size()) - 1; i >= 0; i--)
		{
			if (m_Colliders[i].Owner == entity)
			{
				// this collider needs deleted
				// remove all relevant data from the multimap
				Delete(m_Colliders[i].ID);

				// now to deal with the data in the colliders vector
				// as not to mess up the majority of the indices, we want to swap
				// this item with the last item in the array
				// unless this item is already at the end
				if (i == m_Colliders.size() - 1)
				{
					// if were at the end, then this is easy
					// we just need to remove the collider and dont
					// need to fix any indices

					// we do not need to do anything here
				}
				else
				{
					// this is more complicated now

					// swap this collider with the one at the end
					// swap index 'i' with index (size - 1)
					size_t last = m_Colliders.size() - 1;
					std::iter_swap(m_Colliders.begin() + i, m_Colliders.begin() + last);

					// now that the colliders have been swapped, we need to fix the indices in the multimap
					// anywhere that mentions the collider at index size - 1, needs to be changed to i
					for (auto& pair : m_PartitionedColliders)
					{
						if (pair.second == last)
						{
							pair.second = i;
						}
					}

					// now everything in the partitioning multimap is referring to the correct collider
					// so its safe to delete the last element in the array
				}


				// since we are not using a range-based for loop
				// and since only the last element is being deleted
				// (which even if we are currently iterating on, will no longer be accessed pass this statement)
				// it is safe to call pop_back from within the loop
				m_Colliders.pop_back();
			}
		}

	}


	void SpatialPartition::Delete(ColliderID id)
	{
		// deletes all entries in the multimap of the collider with ColliderID 'id'
		std::vector<std::unordered_multimap<sf::Vector2i, size_t>::iterator> toErase;
		for (auto it = m_PartitionedColliders.begin(); it != m_PartitionedColliders.end(); it++)
		{
			if (m_Colliders[(*it).second].ID == id)
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
				colliders.push_back(&m_Colliders[itr->second]);
			}
		}
	}


}
