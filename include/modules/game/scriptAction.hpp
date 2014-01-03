#ifndef AWSCRIPTACTION_HPP
#define AWSCRIPTACTION_HPP

#include <SFML/System/Time.hpp>

namespace aw
{
	enum ScriptType
	{
		NOTHING,
		CHECKPOINT,
		CHANGE_SPEED,
		CHANGE_GRAVITY,
		ROTATE_CAMERA,
		ZOOM,
		CAMERA_OFFSET,
		SHAKE_CAMERA,
		TILT_X,
		TILT_Y
	};

	//Forward declaration
	class Player;
	class Camera;
	//-------------------

	class ScriptAction
	{
	public:
		ScriptAction();
		ScriptAction(ScriptType type, const sf::Time &duration = sf::Time::Zero, float value1 = 0.f, float value2 = 0.f);

		void upate(const sf::Time &frameTime, Player &player, Camera &camera);

		//Will return if the action is finished
		bool isFinished();

	private:

	private:
		ScriptType m_type;
		sf::Time m_elapsedTime;
		sf::Time m_duration;
		float m_value1;
		float m_value2;
	};
}

#endif