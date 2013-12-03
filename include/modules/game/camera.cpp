#include "camera.hpp"

namespace aw
{
	Camera::Camera()
	{
		m_defaultView = sf::View(sf::FloatRect(0, 0, 800, 450));
		m_gameView = m_defaultView;
	}

	void Camera::update(const sf::Vector2f &playerPosition)
	{
		//Automatic camera
		m_gameView.setCenter(sf::Vector2f(playerPosition.x + 200, playerPosition.y));
	}

	const sf::View &Camera::getDefaultView() const
	{
		return m_defaultView;
	}

	const sf::View &Camera::getGameView() const
	{
		return m_gameView;
	}

	//Script Actions

	void Camera::flip()
	{
		m_gameView.rotate(180);
	}

	void Camera::rotate(float angle)
	{
		m_gameView.rotate(angle);
	}

	void Camera::zoom(float factor)
	{
		m_gameView.zoom(factor);
	}
}