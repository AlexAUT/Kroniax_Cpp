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
		FLIP_CAMERA,
		ZOOM,
		FLICKERING,
		INVERT_COLOR,
		COLOR_OVERLAY
	};

	//Forward declaration
	class Player;
	class Camera;
	//-------------------

	class ScriptAction
	{
	public:
		ScriptAction();
		ScriptAction(ScriptType type, const sf::Time &duration, float value);

		void upate(const sf::Time &frameTime, Player &player, Camera &camera);

		//Will return if the action is finished
		bool isFinished();

	private:

	private:
		ScriptType m_type;
		sf::Time m_elapsedTime;
		sf::Time m_duration;
		float m_value;
	};
}

#endif