#pragma once

#include "LightSystem.h"

namespace SFMLEngine {

	class PointLightSystem : public LightSystem
	{
	public:
		PointLightSystem() = default;
		~PointLightSystem() = default;

		void UploadAllLightingData() override;
		void UploadLightCountData() override;
	};

}
