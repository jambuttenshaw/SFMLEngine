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


}
