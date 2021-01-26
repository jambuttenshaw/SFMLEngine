#pragma once

#include <SFML/Graphics.hpp>

namespace SFMLEngine {

	class Layer
	{
	public:
		virtual ~Layer() {};

		virtual void Update(float timestep) = 0;

		virtual void OnEvent(sf::Event event) = 0;


		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
	};

}
