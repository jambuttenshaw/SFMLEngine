# SFMLEngine

SFMLEngine is a game development framework based upon [SFML](https://www.sfml-dev.org/index.php).

## Features


### Entity Component System
SFMLEngine's Entity Component System is the method through which individual game objects can be constructed as an aggregation of individual components.
For example, to describe an entity's position in the world, a Transform component can be added. For this entity to display an image that is rendered to the display, a SpriteRenderer component can be added.



### Scene System
A scene is a collection of entities and their components, and is typically used to represent one 'screen' or 'level' in a game.
SFMLEngine supports the creation, deletion and loading of scenes.
Upon the scenes construction, it builds all of the entities that make up the content of the scene.



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

All debug tools are implemented as macros, and are stripped from release builds for the optimal performance. Unfortunately this makes release builds significantly more difficult to debug.



### Rigidbodies & Physics System



### Collision and Spatial Partitioning



### Animation System



### Scriptable Entities



### Input System



### Camera System



### Entity layers and tagging



### Spacial Audio System



### GUI System



### Font Library



### Tilemaps and Tile palettes


