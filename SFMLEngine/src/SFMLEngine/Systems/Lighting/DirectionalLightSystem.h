#pragma once

#include "../../ECS/Coordinator.h"


namespace SFMLEngine {

	class DirectionalLightSystem : public System
	{
	public:
		DirectionalLightSystem() = default;
		~DirectionalLightSystem() = default;

		void Init(Coordinator* coordinator)
		{
			m_Coordinator = coordinator;
		}

		void EntityAddedToSystem(Entity entity) override;
		void EntityRemovedFromSystem(Entity entity) override;

		void UploadAllLightingData();
		void UploadLightCountData();
		bool IsStatic(Entity entity);
		void UploadStaticLight(Entity entity, int index);

	private:
		Coordinator* m_Coordinator = nullptr;

		int m_LightCount = 0;
		int m_StaticLightCount = 0;
	};

}
