#ifndef AWCAMERA_HPP
#define AWCAMERA_HPP

#include <SFML/Graphics/View.hpp>

namespace aw
{
	class Camera
	{
	public:
		Camera();

		void update(const sf::Vector2f &playerPosition);

		const sf::View &getDefaultView() const;
		const sf::View &getGameView() const;

		//Script Actions
		void flip();
		void rotate(float angle);
		void zoom(float factor);
		void moveOffset(const sf::Vector2f &val);

	private:

	private:
		sf::View m_defaultView; //For gui etc
		sf::View m_gameView; //For map/player/scripts etc

		float m_zoomFactor;
		sf::Vector2f m_offset; //For script, will move the camera
	};
}

#endif //AWCAMERA_HPP