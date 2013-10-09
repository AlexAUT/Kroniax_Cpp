#ifndef AWSCRIPTMANAGER_HPP
#define AWSCRIPTMANAGER_HPP

#include <vector>

namespace aw
{
	class Player;
	class Camera;

	enum ScriptType
	{
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
		void update(Player &player, Camera &camera);

		void load(const std::string &path);

	private:

		void checkPointAction(Player &player);
		void changeSpeedAction(Player &player);
		void changeGravityAction(Player &player);
		void flipCameraAction(Camera &camera);
		void zoomAction(Camera &camera);
		void flickeringAction(Camera &camera);
		void invertColorAction(Camera &camera);
		void colorOverlayAction(Camera &camera);


	private:
		std::vector<Script> m_scripts;
	};
}

#endif //AWSCRIPTMANAGER_HPP