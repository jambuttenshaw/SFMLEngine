#include "LevelFactory.h"


ResourceID LevelFactory::s_TilePaletteID = NULL_RESOURCE_ID;

void LevelFactory::Init()
{
	// load the tile palette through the resource manager
	// the tilepalette is stored in json format as it has complicated tile metadata
	s_TilePaletteID = TilePalette::LoadFromFile("assets/palettes/terrainPalette.json");
}


Entity LevelFactory::CreateGround(Scene* scene, const std::string& path, const std::string& colliderPath)
{
	// create a new ground entity and set its identifier tag and physics layer
	Entity ground = scene->CreateEntity();
	scene->SetEntityLayer(ground, "Ground");
	scene->SetEntityTag(ground, "Ground");

	// add a transform
	// keep it at the origin for simplicity
	scene->AddComponent(ground, Transform{ });

	// add a tilemap

	scene->AddComponent(ground, Tilemap{ s_TilePaletteID, path });
	// the tilemap should use a lit material
	scene->AddComponent(ground, TilemapRenderer{ Material::Create("Lit"), 0 });


	// this object should be solid
	// add a collider
	if (colliderPath != "null")
		// check if the collider data has been generated offline and stored in json
		// if it has we can load from the json file
		scene->AddComponent(ground, TilemapCollider{ colliderPath });
	else
		// otherwise we must generate a new collider
		scene->AddComponent(ground, TilemapCollider{ TilemapCollider::OptimizationLevel::High });
	// tell the collision system this game object has a tilemap collider
	scene->AddComponent(ground, ColliderInfo{ ColliderType::Tilemap });

	// return the handle
	return ground;
}

Entity LevelFactory::CreateJumpThroughPlatforms(Scene* scene, const std::string& path)
{
	// create a new entity and give it an identifier tag
	Entity jumpThroughPlatforms = scene->CreateEntity();
	scene->SetEntityLayer(jumpThroughPlatforms, "JumpThrough");

	// place it at the origin
	scene->AddComponent(jumpThroughPlatforms, Transform{ });

	// load the tilemap from the json file
	scene->AddComponent(jumpThroughPlatforms, Tilemap{ s_TilePaletteID, path });
	// the platforms should be rendered with a lit material
	scene->AddComponent(jumpThroughPlatforms, TilemapRenderer{ Material::Create("Lit"), 0 });

	// add a trigger collider
	// as this object requires more complex collision resolution than the physics system provides by default
	// by making it a trigger the player can deal with collisions as it likes
	TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
	collider.SetTrigger(true);
	// add the collider
	scene->AddComponent(jumpThroughPlatforms, collider);
	scene->AddComponent(jumpThroughPlatforms, ColliderInfo{ ColliderType::Tilemap });

	// return the handle
	return jumpThroughPlatforms;
}

Entity LevelFactory::CreateLadders(Scene* scene, const std::string& path)
{
	// create the ladders game object
	Entity ladders = scene->CreateEntity();
	// set the tag for the ladders
	scene->SetEntityTag(ladders, "Ladder");

	// place at the origin
	scene->AddComponent(ladders, Transform{ });

	// load the tilemap from the json file and use a lit material to render them in the scene
	scene->AddComponent(ladders, Tilemap{ s_TilePaletteID, path });
	scene->AddComponent(ladders, TilemapRenderer{ Material::Create("Lit"), 0 });

	// this object should have a trigger collider so that the player knows when its collided with a ladder
	// fully optimize the collider since theres not that many ladders in the scene
	TilemapCollider collider{ TilemapCollider::OptimizationLevel::High };
	// ladder colliders are a trigger as we definitely do not want the default collision resolution
	collider.SetTrigger(true);
	scene->AddComponent(ladders, collider);
	scene->AddComponent(ladders, ColliderInfo{ ColliderType::Tilemap });

	// return the handle
	return ladders;
}

Entity LevelFactory::CreateCrystals(Scene* scene, const std::string& path)
{
	// create the entity and assign it an identifier tag
	Entity crystals = scene->CreateEntity();
	scene->SetEntityTag(crystals, "Crystals");

	// place at the origin
	scene->AddComponent(crystals, Transform{ });

	// load the tilemap data from the json file
	scene->AddComponent(crystals, Tilemap{ s_TilePaletteID, path });
	// it should be rendered with a lit material
	scene->AddComponent(crystals, TilemapRenderer{ Material::Create("Lit"), 0 });

	// we need every crystal to have a unique collider so we do not optimize at all
	TilemapCollider collider{ TilemapCollider::OptimizationLevel::None };
	// crystals are a trigger collider
	// as we do not want default collision resolution
	// instead the player interacts with them in a unique way
	collider.SetTrigger(true);
	scene->AddComponent(crystals, collider);
	scene->AddComponent(crystals, ColliderInfo{ ColliderType::Tilemap });

	return crystals;
}


Entity LevelFactory::CreateLevelEnd(Scene* scene, const sf::Vector2f& position)
{
	// the level end is a trigger that notifies the player it has reached the point it should move onto the next level
	Entity levelEnd = scene->CreateEntity();
	// give it a tag so it can be identified
	scene->SetEntityTag(levelEnd, "LevelEnd");

	// add a transform to position the level end trigger
	scene->AddComponent(levelEnd, Transform{ position });

	// add a trigger collider the size of one tile
	BoxCollider collider{ { 32, 32 }, { 0, 0 } };
	collider.SetTrigger(true);
	scene->AddComponent(levelEnd, collider);
	scene->AddComponent(levelEnd, ColliderInfo{ ColliderType::Box });

	return levelEnd;
}


Entity LevelFactory::CreateTutorialText(Scene* scene, int textID, const sf::Vector2f& position)
{
	// tutoral text describes how to play the game
	// the actual text content is set up by the script
	// so this factory just needs to create an entity, add the script, and call init
	Entity newText = scene->CreateEntity();
	auto& script = scene->AddNativeScript<TutorialText>(newText);
	script.Init(textID, position);

	return newText;
}

Entity LevelFactory::CreateFill(Scene* scene, const std::string& path)
{
	// a 'fill' is just all the black tiles placed around the ground to stop the background from showing around the ground tiles
	// it does not interact with any of the sprites in the game
	Entity fill = scene->CreateEntity();

	// place at origin
	scene->AddComponent(fill, Transform{ });

	// load the tilemap from the file
	scene->AddComponent(fill, Tilemap{ s_TilePaletteID, path });
	scene->AddComponent(fill, TilemapRenderer{ Material::Create("Lit"), 0 });

	return fill;
}