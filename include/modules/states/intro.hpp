#ifndef AWINTRO_HPP
#define AWINTRO_HPP

#include "state.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace aw
{
	class Intro : public State
	{
	public:
		Intro(StateMachine &stateMachine);

		void update(const sf::Time &frameTime);
		void render(sf::RenderWindow &window);

	private:

	private:
		float m_timeActive;

		sf::Texture m_texLogo;
		sf::Sprite m_sprLogo;

	};
}

#endif