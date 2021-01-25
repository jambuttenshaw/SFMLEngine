#include <SFMLEngine.h>
#include <SFMLEngine/EntryPoint.h>

class SandboxApp : public SFMLEngine::Application
{

};

SFMLEngine::Application* SFMLEngine::CreateApplication()
{
	return new SandboxApp;
}
