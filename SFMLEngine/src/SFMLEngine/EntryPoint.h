#pragma once

extern SFMLEngine::Application* SFMLEngine::CreateApplication();

int main(int argc, char** argv)
{

	SFMLEngine::Log::Init();

	SFMLEngine::Application* app = SFMLEngine::CreateApplication();

	app->Run();

	delete app;

	return 0;
}
