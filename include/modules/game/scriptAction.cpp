#include "scriptAction.hpp"

#include "player.hpp"
#include "camera.hpp"

namespace aw
{
	namespace priv
	{
		void changeSpeed(float percentage, float value, Player &player);
		void changeGravity(float percentage, float value, Player &player);
		void rotateCamera(float percentage, float value, Camera &camera);
		void zoomCamera(float percentage, float value, Camera &camera);
		void cameraOffset(float percentage, float xValue, float yValue, Camera &camera);
		void tiltX(float percentage, Player &player);
		void tiltY(float percentage, Player &player);
	}

	//--------------------------------------------------------------------------------------------------------

	ScriptAction::ScriptAction():
		ScriptAction(ScriptType::NOTHING, sf::Time::Zero, 0, 0)
	{

	}

	ScriptAction::ScriptAction(ScriptType type, const sf::Time &duration, float value1, float value2) :
		m_type(type),
		m_elapsedTime(sf::Time::Zero),
		m_duration(duration),
		m_value1(value1),
		m_value2(value2)
	{

	}

	void ScriptAction::upate(const sf::Time &frameTime, Player &player, Camera &camera)
	{
		//The percantage of the frameTime from the duration
		float percentage = 0.f;
		m_elapsedTime += frameTime;
		//Calculate the full frameTime of the "new" elapsed time < duration
		if (m_duration == sf::Time::Zero)
		{
			percentage = 1.f;
		}
		else if (m_elapsedTime <= m_duration)
		{
			percentage = frameTime.asSeconds() / m_duration.asSeconds();
		}
		else
		{
			//subtract the overtime
			sf::Time difference = frameTime - (m_elapsedTime - m_duration);
			percentage = difference.asSeconds() / m_duration.asSeconds();
		}

		switch (m_type)
		{
		case ScriptType::CHANGE_SPEED: priv::changeSpeed(percentage, m_value1, player); break;
		case ScriptType::CHANGE_GRAVITY: priv::changeGravity(percentage, m_value1, player); break;
		case ScriptType::ROTATE_CAMERA: priv::rotateCamera(percentage, m_value1, camera); break;
		case ScriptType::ZOOM: priv::zoomCamera(percentage, m_value1, camera); break;
		case ScriptType::CAMERA_OFFSET: priv::cameraOffset(percentage, m_value1, m_value2, camera); break;
		case ScriptType::TILT_X: priv::tiltX(percentage, player); break;
		case ScriptType::TILT_Y: priv::tiltY(percentage, player); break;
		default: break;
		}
	}

	bool ScriptAction::isFinished()
	{
		return (m_elapsedTime >= m_duration);
	}

	//--------------------------------------------------------------------------------------------------------

	namespace priv
	{
		void changeSpeed(float percentage, float value, Player &player)
		{
			player.setSpeedX(player.getSpeed().x + (value * percentage));
			//std::cout << "Player speed: " << player.getSpeed().x << std::endl;
		}
		void changeGravity(float percentage, float value, Player &player)
		{
			player.setGravitation(player.getGravitation() + (value * percentage));
			//std::cout << "Player gravity: " << player.getGravitation() << std::endl;
		}
		void rotateCamera(float percentage, float value, Camera &camera)
		{
			camera.rotate(value * percentage);
		}
		void zoomCamera(float percentage, float value, Camera &camera)
		{
			camera.zoom(value * percentage);
		}
		void cameraOffset(float percentage, float xValue, float yValue, Camera &camera)
		{
			camera.moveOffset(sf::Vector2f(xValue * percentage, yValue * percentage));
		}
		void tiltX(float percentage, Player &player)
		{
			player.setSpeedX(0);
		}
		void tiltY(float percentage, Player &player)
		{
			player.setSpeed({ player.getSpeed().x, 0.f });
		}
	}
}