#include "scriptManager.hpp"

#include "player.hpp"
#include "camera.hpp"

#include <fstream>
#include <sstream>

namespace aw
{
	void ScriptManager::update(Player &player, Camera &camera)
	{
		//Check for Scripts
		for (auto &it : m_scripts)
		{
			if (!it.used)
			{
				if (player.getVertexPosition(0).x >= it.xPos * 25.f) //Front vertex
				{
					it.used = true;

					switch (it.type)
					{
					case CHECKPOINT: checkPointAction(player); break;
					case CHANGE_SPEED: changeSpeedAction(player); break;
					case CHANGE_GRAVITY: changeGravityAction(player); break;
					case FLIP_CAMERA: flipCameraAction(camera); break;
					case ZOOM: zoomAction(camera); break;
					case FLICKERING: flickeringAction(camera); break;
					case INVERT_COLOR: invertColorAction(camera); break;
					case COLOR_OVERLAY: colorOverlayAction(camera); break;
					default: break;
					}
				}
			}
		}
	}

	void ScriptManager::load(const std::string &path)
	{
		std::fstream file;

		if (!file.good())
			return;

		std::string line;
		while (std::getline(file, line))
		{
			if (line == "[Scripts]")
			{
				while (std::getline(file, line))
				{
					if (line == "[/Scripts]")
					{
						break;
					}
					 
					int type;
					int xPos;
					float first, second, third;

					std::stringstream sstr(line);
					sstr >> type >> xPos >> first >> second >> third;

					m_scripts.push_back(Script(ScriptType(type), xPos, first, second, third));
				}
			}
		}

		file.close();
	}


	////////ACTION FUNCTIONS//////////////////////////////////////////////////////////

	void ScriptManager::checkPointAction(Player &player)
	{

	}

	void ScriptManager::changeSpeedAction(Player &player)
	{

	}
	void ScriptManager::changeGravityAction(Player &player)
	{

	}
	void ScriptManager::flipCameraAction(Camera &camera)
	{

	}
	void ScriptManager::zoomAction(Camera &camera)
	{

	}
	void ScriptManager::flickeringAction(Camera &camera)
	{

	}
	void ScriptManager::invertColorAction(Camera &camera)
	{

	}
	void ScriptManager::colorOverlayAction(Camera &camera)
	{

	}
}