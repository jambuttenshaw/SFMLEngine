#pragma once

#include <inttypes.h>
#include <bitset>

namespace SFMLEngine {

	// ---------
	// ECS types
	// ---------

	// simple type alias, as entities are just ID's
	using Entity = uint32_t;

	// the maximum entities a scene can contain
	// used to define the size of arrays
	const Entity MAX_ENTITIES = 5000;
	const Entity INVALID_ENTITY_ID = MAX_ENTITIES + 1;


	// a type alias for defining component types
	// a component is represented internally by an unsigned 8bit int
	using ComponentType = uint8_t;

	// used to define size of arrays later on
	const ComponentType MAX_COMPONENTS = 32;


	// a bit field that represents what components an entity has attatched
	// each entity will have an attatched signature
	using Signature = std::bitset<MAX_COMPONENTS>;



	// -------------------------
	// Resource Management Types
	// -------------------------

	// used as the key to the resource manager's resources
	using ResourceID = uint32_t;
	// the maximum number of resources the manager can manage
	const ResourceID MAX_RESOURCES = 5000;

	// since we store lots of different types of resources
	// and they dont inherit from a common base class
	using ResourceHandle = void*;

	// in case a texture wasnt, or couldnt be, loaded
	const ResourceID NULL_RESOURCE_ID = MAX_RESOURCES + 1;



	// --------
	// Lighting
	// --------

	const int MAX_POINT_LIGHTS = 4;
	const int MAX_DIRECTIONAL_LIGHTS = 2;



	// --------
	// TILEMAPS
	// --------

	using TileID = uint8_t;
	const TileID MAX_TILES = 254;
	const TileID NULL_TILE_ID = 255;

}
