#ifndef AWCHATHANDLER_HPP
#define AWCHATHANDLER_HPP

#include "../sfchat/sfchat.h"
#include "../messageBus/receiver.hpp"

#include <array>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace aw
{
	class MessageBus;

	class ChatHandler : public ::sfchat::Chat, public sf::Drawable, public Receiver
	{
	public:
		ChatHandler(MessageBus &messageBus);

		void updateVisual();

		void receiveMessage(const Message &msg);

	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		bool mVisible;

		MessageBus &m_messageBus;

		sf::Font mFont;
		sf::Text mInputText;
		std::array<sf::Text, 13> mHistory;
		sf::RectangleShape mSourundingRect;
		sf::RectangleShape mInputRect;
	};
}

#endif