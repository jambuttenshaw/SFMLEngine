#pragma once

extern SFMLEngine::Application* SFMLEngine::CreateApplication();

int main(int argc, char** argv)
{

	SFMLEngine::Application* app = SFMLEngine::CreateApplication();

	app->Run();

	delete app;

	return 0;
}
