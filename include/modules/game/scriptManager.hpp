#ifndef AWSCRIPTMANAGER_HPP
#define AWSCRIPTMANAGER_HPP

#include <vector>
#include <list>
#include <memory>

#include "scriptAction.hpp"
#include "player.hpp"
#include "camera.hpp"

namespace sf
{
	class RenderWindow;
}

namespace aw
{
	class Player;
	class Camera;

	struct Checkpoint
	{
		//It stores two copies of the elemets...
		Player savedPlayer;
		Camera savedCamera;
		std::list<ScriptAction> savedScriptActions;
	};

	struct Script
	{
		//Scripttype included from scriptAction.hpp
		ScriptType type;
		int xPos;
		float first, second, third;
		bool used;

		Script(ScriptType ptype, int pxPos, float pfirst, float psecond, float pthird) :
			type(ptype), xPos(pxPos), first(pfirst), second(psecond), third(pthird), used(false)
		{}
	};

	class ScriptManager
	{
	public:
		ScriptManager();

		void update(const sf::Time &frameTime, Player &player, Camera &camera);

		void load(const std::string &path, bool online = false);

		void render(sf::RenderWindow &window);

		Checkpoint* getLastCheckPoint();

		void deleteScripts();
		//Set resetCheckpoints to false if you do not want to reset their state
		//(when respawning the player to a checkpoint)
		void resetScriptStates(bool resetCheckpoints = true);

	private:

		void checkPointAction(Player &player, Camera &camera);
		void changeSpeedAction(float value, float duration);
		void changeGravityAction(float value, float duration);
		void rotateCamera(float value, float duration);
		void zoomAction(float value, float duration);
		void cameraOffset(float xValue, float yValue, float duration);


	private:
		std::vector<Script> m_scripts;
		std::list<ScriptAction> m_scriptActions;

		std::unique_ptr<Checkpoint> m_checkpoint;
	};
}

#endif //AWSCRIPTMANAGER_HPP