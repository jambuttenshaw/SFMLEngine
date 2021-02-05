#pragma once

#include "LightSystem.h"


namespace SFMLEngine {

	class DirectionalLightSystem : public LightSystem
	{
	public:
		DirectionalLightSystem() = default;
		~DirectionalLightSystem() = default;

		void UploadAllLightingData() override;
		void UploadLightCountData() override;
	};

}
