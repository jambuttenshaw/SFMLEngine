#include "LightSystem.h"


namespace SFMLEngine {

	void LightSystem::EntityAddedToSystem(Entity entity)
	{
		m_LightCount++;
	}

	void LightSystem::EntityRemovedFromSystem(Entity entity)
	{
		m_LightCount--;
	}

}