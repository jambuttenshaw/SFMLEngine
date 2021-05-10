# SFMLEngine

SFMLEngine is a game development framework based upon [SFML](https://www.sfml-dev.org/index.php).

## How To Build

The following instructions are how to build SFMLEngine and Crystal Caverns for Windows using 

1. Clone this repo.
2. Run `Win-GenProjects.bat`. This will generate a `.sln` file to be opened with Visual Studio 2019. This sets up all of the dependencies, include paths, etc.
3. Make sure the start-up project is set to Crystal Caverns.
4. Build the Crystal Caverns project (SFMLEngine will be built first automatically as it is a dependency).


## Features


### Entity Component System
SFMLEngine's Entity Component System is the method through which individual game objects can be constructed as an aggregation of individual components.
For example, to describe an entity's position in the world, a Transform component can be added. For this entity to display an image that is rendered to the display, a SpriteRenderer component can be added.

Components are added/removed/retrieved from entities through a Scene or a Scriptable Entity.



### Scene System
A scene is a collection of entities and their components, and is typically used to represent one 'screen' or 'level' in a game.
SFMLEngine supports the creation, deletion and loading of scenes.
Upon the scenes construction, it builds all of the entities that make up the content of the scene.

The scene handles the creation, deletion and management of its entities. When the scene is destroyed it automatically cleans up its own entities and their components.
Do note that if you manually delete an entity, it does not get deleted immediately but instead at the beginning of the next frame. This is to avoid causing errors through deleting partially-processed entities.

SFMLEngine offers two types of scene loading: single and additive.
Single scene loading replaces all currently loaded scenes with the new scene.
Additive scene loading leaves all currently loaded scenes in place, and loads itself alongside the existing scenes.

Scene loading and deletion does not happen immediately: after a function has been called to load or delete a scene, it is queued to execute at the beginning of the next frame.
This is done to avoid the situation where halfway through updating scene 1, scene 2 is loaded and scene 1 is deleted. But the engine is still halfway through processing scene 1, so all of the entities it is processing have been deleted... a very unfortunate situation.



### Resource Manager
The resource manager allows the user to easily share resources between different game objects.
For example, the user wants to have 5 different game objects that all display the same texture. The texture is loaded once through the resource manager, and then an internal ID for the texture is return following any request to obtain the resource.

Most types of resources, Textures, Materials, Tile palettes, Audio, etc, offer a factory function that automatically loads them through the Resource Manager. The user should rarlely have to interact with the resource manager itself, apart from retrieving pointers to the resources themselves.



### Material & Shader System
SFMLEngine implements a powerful material and shader system to allow easy creation of interesting graphical effects.
SFMLEngine offers a shader library of SFML's sf::Shader objects, that can be loaded from shader source files created by the user. The engine itself offers a variety of standard shaders for most purposes, such as basic unlit sprites and sprites lit by point or directional lights, with added support of normal maps to simulate more detailed geometry.

SpriteRenderers and TilemapRenderers can be assigned any material, and the sprite/tilemap will be rendered with the shader referenced by the material and the shader data contained by the material. Materials are shared between objects to minimise the amount of uniform loading. If you want to assign an entity a unique material, SFMLEngine supports this through the material system's CreateInstance functionality.

Be warned that using a wide variety of materials will quickly detriment performance. Keep the number of materials in use to a minimuim for the best performance.



### Lighting System
SFMLEngine provides two types of lights: point and directional.

Point lights irradiate outward from the position of the entity, and have an intensity, range and colour. Directional lights are not associated with a position, but instead have a constant direction that every fragment will percieve the light to come from. As they do not have a position, naturally the intensity of directional lights does not diminish with distance.

Both types of lights can be marked as 'static', which avoids the uploading of lighting data after the lights creation. This is optimal for performance if you know the lights properties will never be modified, but this does impose limitations on the lights use.

There is currently a maximum number of lights allowed in any one scene. Exceeding this value will produce an error message and cause undefined behaviour.



### Maths Tools
SFMLEngine provides helpful functions for dealing with 2 and 3 dimensional vectors, such as finding the magnitude, square magnitude, normalizing, min, max, etc...
It also provides helpful functions for dealing with rectangles, such as finding the intersection between two rectangles, or finding the centroid.
Finally the maths tools provide functions for the generation of random numbers or vectors.



### Debugging Tools
SFMLEngine implements a plethora of helpful debugging tools.
Primarily, it provides logging functions with a variety of logging levels. The logging functionality is based on the excellent library [spdlog](https://github.com/gabime/spdlog). Two sets of logging macros are provided, one with the format `LOG_CORE_{priority}` and another `LOG_{priority}`. The 'Core' macros should be reserved for internal engine use only. In your client application, use the other ones.

SFMLEngine also provides its own implementation of assertions. Assertions follow a similar format to logging functions, and similarly offer two individual core and client macros.

SFMLEngine also provides graphical debug tools. It allows for the drawing of rectangles and lines in an overly on the game world, and allows the output of data to the HUD in runtime for easy observation.

All debug tools are implemented as macros, and are stripped from release builds for the optimal performance. Consequently this makes release builds significantly more difficult to debug.



### Rigidbodies & Physics System
SFMLEngine contains a very simple physics system, that simulates the effects of forces, acceleration and velocity on a body. It does not include dynamic rotation. Physics can be applied to any object through the addition of a Rigidbody component, which allows the client to interact with the physical properties of the entity.

The physics system automatically handles the dynamics simulation of a rigidbody, as well as collision detection and resolution between other colliders. The physics system also handles sending out collision callbacks to [Scriptable Entities](#scripts).



### Transforms
Every entity that has a position in a game world requires a transform. A transform describes the position, rotation and scale of an object, and also allows this to be specified relative to a 'parent' transform. Transforms also provide matrices for converting between local and global coordinates spaces.



### Collision and Spatial Partitioning
The collision system is responsible for detecting when two colliders overlap, and gathering useful information regarding their overlap. The collision system can be queried through the `SFMLEngine::Physics` module. 

3 collision primitives are offered:
- Box Colliders
- Circle Colliders
- Tilemap Colliders

The data collected from a collision includes:
- The general direction in which it occurred (up/down/left/right)
- The centroid of the collision
- The other entity's Entity ID.
- The bounds of this entity's collider
- The bounds of the other entity's collider
- The ID of the other entity's collider, for the case where the other entity is constructed from numerous colliders
- Whether the collider is a trigger collider or a regular collider

The physics system does not automatically resolve collisions between triggers and rigidbodies, they are used for sending **callback only** when a collision occurs.
Callback from triggers are sent to seperate callback functions from regular collider callback functions.

Some scenes may contain a very large amount of colliders, so SFMLEngine provides a way to optimise this situation through a spacial partitioning system.
Spacial partitioning divides the world space into equally sized chunks and assigns every collider in the game world into one or more chunks.
Then, when the collision system is queried, all partitions in which the current rigidbody lies are searched to find any colliders that share a partition. Thus instead of having to check collision against every collider in the game world, we only have to check colliders that share a partition.


The spacial partitioning system still contains some serious flaws currently, but due to them not having an impact on the Crystal Caverns project they were left to be resolved at a later date. These issues include:
- Moving objects (through either rigidbody dynamics or simply by manipulation of the entity's transform) do not get their position updated in the spacial partitioning data structure. This means that collisions between two dynamic objects are unlikely to be detected.
- Fixed partition size. The partition size has an annoying dependency upon tilemap colliders. Tilemap colliders have a maximum size to which they can be optimized, which is in place to make sure that every collider can fit inside no more than 4 partitions at one time. Unfortunately, instead of the partition size defining the maximum size of the tilemap collider, it goes the other way round and the spacial partition becomes dependent on the tile size and optimization properties used on the tilemaps in the game.
- Large box colliders ( larger that 2 partitions on at least one axis) will not be registered in the spacial partition correctly.



### Animation System
The animation system allows for the animation of sprites. To register with the animation system, an animator component is attached to the entity. This animator component can be provided with any number of individual animations, that can contain any number of individual frames. 

The animator provides tools for playing, pausing, stopping, flipping, switching between animations. Then the processing of the animation and updating the graphical appearance of the animated sprite is handled internally by the animation system.



### Scriptable Entities
Scriptable entities allow for interaction to be added in to a game built with SFMLEngine. A scriptable entity is a script created by the client that subscribes to events happening within the engine. The events a scriptable entity can subscribe to include:
- The game start (or the script's creation, if it was created after the game had started)
- The update process that happens every frame
- Collision and trigger callbacks
- When a new scene is loaded

Scriptable entities also allow for client scripts to interact with SFMLEngine's entity component system, by providing functionality to:
- Create and destroy entities (including itself)
- Add components to itself or other entities
- Remove components from itself or other entities
- Get components from itself or other entities
- Add scripts to itself or other entities
- Remove scripts from itself or other entities
- Find all entities with a certain tag
- Get the tag of a specific entity
- Get the name of a specific entity
- Get the layer of a specific entity

Using these tools plus all of the other modules offered by SFMLEngine, a scriptable entity can add interactivity and dynamism to any game object within the game.



### Input System
SFMLEngine provides tools that make interacting with the keyboard and mouse as simple as possible.
It allows for polling if keys have just been pressed this frame, or if they have been held for any amount of time, and similarly for the mouse buttons.

It also detects mouse movement and provides the mouse delta value. Similarly, it calculates and provides the scroll wheel delta.

The input system provides two functions for getting the mouse's position: one for the screen position, one for the world position.
The screen position of the mouse is naturally its position relative to the window, with (0,0) at the topleft. The world position casts the mouse position into the game world with respect to the scene's current main camera. This is very useful for finding when the mouse interacts with objects inside the game world, a very difficult task when the mouse pointer and object are in two different coordinate spaces!



### Camera System
SFMLEngine provides a camera system to allow for easy and dynamic viewing of complex game worlds. All scenes **must** contain a camera. Camera's can be marked as 'main', which tells the engine that the scene is to be rendered from this camera's point of view. Rendering from multiple cameras to multiple render targets is currently not supported directly by SFMLEngine. 



### Entity layers and tagging
It is often very helpful to sort and uniquely identify entities. To achieve this, entities can be assigned names, tags, and layers.

Names are symbolic only and do not perform any role in the engines operation only, but they may be helpful for debugging.

Tags, however, are extremely useful for allowing one entity to get a reference to another. The ECS can be queried through the `FindAllEntitiesWithTag` function in either a Scene or ScriptableEntity, which returns a vector of all entities assigned with that tag. If you know the tag you are searching for is unique, then your desired entity is always the first item in that vector. 

Layers are also used within the engine. The physics system makes use of layers. The client can define choose to ignore collisions between layers, either for game design purposes or optimisations. For example, only the player will ever be looking to collide with a collectible object, so collisions between all entities in the Enemy layer and the Collectible layer can be safely ignored.


### Spacial Audio System
SFMLEngine doesn't provide any additional functionality on top of SFML's spacial audio system, but instead wraps around it and loads all sound resources through the resource manager. This assures that no audio data is duplicated in memory. Multiple sounds can easily be created that use the same audio data but with different world positions, volumes, etc, but only 1 instance of that audio data will be loaded into memory.

The audio system has a 'Listener', which defines where all sounds are percieved to have come from. Each individual sound can have a position, and other properties to define how they are heard. Of course, sounds can optionally be set to be produced relative to the listener, and therefore do not produce spacial audio effects.


### GUI System



### Font Library 



### Tilemaps



### Tile Palettes



### Tilemap Colliders

