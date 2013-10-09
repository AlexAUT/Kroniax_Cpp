#include "scriptManager.hpp"

#include <fstream>
#include <sstream>

namespace aw
{
	ScriptManager::ScriptManager() :
		m_checkpoint(nullptr)
	{}

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
					case CHECKPOINT: checkPointAction(player, camera); break;
					case CHANGE_SPEED: changeSpeedAction(player, it.first); break;
					case CHANGE_GRAVITY: changeGravityAction(player, it.first); break;
					case FLIP_CAMERA: flipCameraAction(camera); break;
					case ZOOM: zoomAction(camera, it.first); break;
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
		std::fstream file(path.c_str(), std::ios::in);

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


	Checkpoint *ScriptManager::getLastCheckPoint()
	{
		if (m_checkpoint)
		{
			return &*m_checkpoint;
		}

		return nullptr;
	}

	////////ACTION FUNCTIONS//////////////////////////////////////////////////////////

	void ScriptManager::checkPointAction(Player &player, Camera &camera)
	{
		m_checkpoint = std::unique_ptr<Checkpoint>(new Checkpoint());
		m_checkpoint->savedPlayer = player;
		m_checkpoint->savedCamera = camera;
	}

	void ScriptManager::changeSpeedAction(Player &player, float first)
	{
		player.setSpeedX(first);
	}
	void ScriptManager::changeGravityAction(Player &player, float first)
	{
		player.setGravitation(first);
	}
	void ScriptManager::flipCameraAction(Camera &camera)
	{
		
	}
	void ScriptManager::zoomAction(Camera &camera, float first)
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