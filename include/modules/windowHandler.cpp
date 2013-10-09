#include "windowHandler.hpp"

#include "../messageBus/messageBus.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

namespace aw
{
	WindowHandler::WindowHandler(MessageBus &msgBus) :
		m_messageBus(msgBus)
	{
		m_messageBus.addReceiver(this);
	}

	void WindowHandler::checkEventQueue()
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				Message msg(std::hash<std::string>()("window resized"));
				msg.push_back(event.size.width);
				msg.push_back(event.size.height);
				m_messageBus.sendMessage(msg);
			}
			else if (event.type == sf::Event::LostFocus)
			{
				m_messageBus.sendMessage(Message(std::hash<std::string>()("lost focus")));
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				m_messageBus.sendMessage(Message(std::hash<std::string>()("gained focus")));
			}
			else
			{
				Message msg(std::hash<std::string>()("event"));
				msg.push_back(event);
				m_messageBus.sendMessage(msg);
			}
		}
	}

	void WindowHandler::receiveMessage(const Message &msg)
	{
		if (msg.ID == std::hash<std::string>()("window settings"))
		{
			std::string name = static_cast<Value<std::string>*>(&*msg.m_values[0])->m_value;

			sf::VideoMode mode;
			mode.width = *msg.getValue<unsigned int>(1);
			mode.height = *msg.getValue<unsigned int>(2);

			sf::Uint8 style;
			if (*msg.getValue<bool>(3))
			{
				style = sf::Style::Fullscreen;
				mode = sf::VideoMode::getDesktopMode();
			}
			else
			{
				style = sf::Style::Default;
			}

			sf::ContextSettings settings;
			settings.antialiasingLevel = *msg.getValue<unsigned int>(4);
			settings.majorVersion = *msg.getValue<unsigned int>(5);
			settings.minorVersion = *msg.getValue<unsigned int>(6);

			m_window.create(mode, name, style, settings);
			m_window.setVerticalSyncEnabled(true);

			//Set view
			sf::View view;
			view.setSize(sf::Vector2f(*msg.getValue<float>(7), *msg.getValue<float>(8)));
			view.setCenter(view.getSize() / 2.f);
			m_window.setView(view);
		}
		else if (msg.ID == std::hash<std::string>()("close game"))
		{
			m_window.close();
		}
	}

	sf::RenderWindow &WindowHandler::getWindow()
	{
		return m_window;
	}
}