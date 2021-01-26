#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h>


class GameLayer : public SFMLEngine::Layer
{
public:
	GameLayer()
		: Layer()
	{

		
	}

	~GameLayer() {}

	void GameLayer::OnAttach() {}
	void GameLayer::OnDetach() {}

	void GameLayer::OnEvent(sf::Event) {}

	void GameLayer::Update(float timestep)
	{

	}

private:

};



class SandboxApp : public SFMLEngine::Application
{
public:
	SandboxApp()
		: Application()
	{
		PushLayer(new GameLayer());
	}
};

SFMLEngine::Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
