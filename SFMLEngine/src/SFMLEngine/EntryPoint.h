#pragma once


/*
The entry point is defined within the engine,
and then included into the users program.

It isn't sensible for the user to have to define the entry point,
they should only be defining their application; not how it should be run.
*/

int main()
{
#ifdef SFMLENGINE_DEBUG
	SFMLEngine::Log::Init();
#endif
	SFMLEngine::Application* app = SFMLEngine::CreateApplication();

	app->Run();

	delete app;

#ifdef SFMLENGINE_DEBUG
	_CrtDumpMemoryLeaks();
#endif
}
