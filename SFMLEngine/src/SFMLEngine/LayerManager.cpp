#include "LayerManager.h"

#include "Core.h"

#include "SFMLEngine/Systems/Physics/Physics.h"


namespace SFMLEngine {

	uint8_t LayerManager::s_LayerCount = 0;
	std::unordered_map<std::string, Layer> LayerManager::s_Layers;

	Layer LayerManager::LayerFromString(const std::string& layerName)
	{
		// make sure we dont have too many layers once this new one is created
		SFMLE_CORE_ASSERT(s_LayerCount + 1 < MAX_LAYERS, "Too many layers!");

		
		// check if layerName already exists
		auto& exists = LayerExists(layerName);
		// if it exists return that layer
		if (exists.first) return exists.second;


		// create a new layer
		// flag the bit in the index of the layer count
		Layer newLayer;
		newLayer.set(s_LayerCount);

		// increment layer count
		s_LayerCount++;

		// insert into the layers dict
		s_Layers.insert(std::make_pair(layerName, newLayer));
		
		// register this new layer with the physics system
		Physics::AddPhysicsLayer(newLayer);

		// return the new layer
		return newLayer;
	}

	Layer LayerManager::GetLayer(const std::string& layerName)
	{
		// get the information about the layer
		auto& layer = LayerExists(layerName);
		// if it doesnt exist give the user a warning
		if (!layer.first) LOG_CORE_WARN("Attempting to retrieve non-existant layer '{0}'", layerName);
		// return the layer
		// this will be an empty layer if it doesnt exist
		return layer.second;
	}

	const std::string& LayerManager::GetLayerName(Layer layer)
	{
		for (auto const& pair : s_Layers)
		{
			if (pair.second == layer)
				return pair.first;
		}

		SFMLE_CORE_ASSERT(0, "Failed to retrieve layer name.");

		return std::string();
	}

	std::vector<Layer> LayerManager::GetAllLayers()
	{
		std::vector<Layer> layers;
		for (auto& layer : s_Layers)
		{
			layers.push_back(layer.second);
		}
		return layers;
	}


	Layer LayerManager::CreateLayerMaskAdditive(const std::vector<Layer>& layers)
	{
		// starts with an empty layer mask and adds each layer into it

		Layer newLayerMask;
		for (auto const& layer : layers)
		{
			// add each layers signature into the mask
			newLayerMask |= layer;
		}

		return newLayerMask;
	}

	Layer LayerManager::CreateLayerMaskSubtractive(const std::vector<Layer>& layers)
	{
		// starts with a full layer mask and subtracts each layer from it

		// create a layer will all bits set
		Layer newLayerMask = Layer{}.set();
		for (auto const& layer : layers)
		{
			// remove each layers signature from the layer mask
			newLayerMask &= ~layer;
		}

		return newLayerMask;
	}




	std::pair<bool, Layer> LayerManager::LayerExists(const std::string& layerName)
	{
		if (s_Layers.find(layerName) == s_Layers.end())
		{
			// return false and an empty layer
			return std::make_pair(false, Layer{});
		}
		else
		{
			return std::make_pair(true, s_Layers[layerName]);
		}
	}

}