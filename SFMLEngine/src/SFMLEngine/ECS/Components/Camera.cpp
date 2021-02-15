#include "Camera.h"

#include "SFMLEngine/Application.h"

// had to create a cpp file for camera since application was needed,
// and including application in the header will lead to circular includes


namespace SFMLEngine {
	Camera::Camera()
		: Size(Application::GetWindowSize()), Viewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f)), Zoom(1.0f), Main(true)
	{}
}
