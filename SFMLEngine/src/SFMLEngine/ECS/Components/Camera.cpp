#include "Camera.h"

#include "SFMLEngine/Application.h"

// had to create a cpp file for camera since application was needed,
// and including application in the header will lead to circular includes


namespace SFMLEngine {
	Camera::Camera()
		: m_Size(Application::GetWindowSize()), m_Viewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f)), m_Zoom(1.0f), m_Main(true)
	{}
}
