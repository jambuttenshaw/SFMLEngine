#pragma once

#include "Constants.h"

#include <unordered_map>


namespace SFMLEngine {

	class LayerManager
	{
	public:

		// layers
		static Layer CreateLayer(const std::string& layerName);
		static Layer GetLayer(const std::string& layerName);

		// layer masks
		// a layer mask is effectively a bitset containing the signatures of many layers
		// if we want to filter out certain layers
		static Layer CreateLayerMaskAdditive(const std::vector<Layer>& layers);
		static Layer CreateLayerMaskSubtractive(const std::vector<Layer>& layers);


	private:
		static std::pair<bool, Layer> LayerExists(const std::string& layerName);

	private:
		static std::unordered_map<std::string, Layer> s_Layers;

		static uint8_t s_LayerCount;

	};

}
