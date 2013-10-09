#ifndef AWSCRIPTMANAGER_HPP
#define AWSCRIPTMANAGER_HPP

#include <vector>
#include <memory>

#include "player.hpp"
#include "camera.hpp"

namespace aw
{
	class Player;
	class Camera;

	struct Checkpoint
	{
		//It stores two copies of the elemets...
		Player savedPlayer;
		Camera savedCamera;
	};

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

	struct Script
	{
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

		void update(Player &player, Camera &camera);

		void load(const std::string &path);

		Checkpoint* getLastCheckPoint();

	private:

		void checkPointAction(Player &player, Camera &camera);
		void changeSpeedAction(Player &player, float first);
		void changeGravityAction(Player &player, float first);
		void flipCameraAction(Camera &camera);
		void zoomAction(Camera &camera, float first);
		void flickeringAction(Camera &camera);
		void invertColorAction(Camera &camera);
		void colorOverlayAction(Camera &camera);


	private:
		std::vector<Script> m_scripts;

		std::unique_ptr<Checkpoint> m_checkpoint;
	};
}

#endif //AWSCRIPTMANAGER_HPP