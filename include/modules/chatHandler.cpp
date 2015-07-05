#include "chatHandler.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "../global.hpp"
#include "../utilities/hash.hpp"
#include "../messageBus/messageBus.hpp"

namespace aw
{
	ChatHandler::ChatHandler(MessageBus &messageBus) :
		mVisible(false),
		m_messageBus(messageBus)
	{
		m_messageBus.addReceiver(this);

		mFont.loadFromFile(Helper::getData("fonts/visitor1.ttf"));

		mSourundingRect.setPosition(sf::Vector2f(5, 145));
		mSourundingRect.setSize(sf::Vector2f(790, 300));
		mSourundingRect.setFillColor(sf::Color::Black);
		mSourundingRect.setOutlineThickness(5);
		mSourundingRect.setOutlineColor(sf::Color::White);

		mInputRect.setPosition(sf::Vector2f(8, 420));
		mInputRect.setSize(sf::Vector2f(783, 22));
		mInputRect.setFillColor(sf::Color::Black);
		mInputRect.setOutlineColor(sf::Color::Blue);
		mInputRect.setOutlineThickness(3);

		mInputText.setFont(mFont);
		mInputText.setPosition(sf::Vector2f(12, 420));
		mInputText.setCharacterSize(15);
		mInputText.setColor(sf::Color::White);

		for (std::size_t i = 0; i < mHistory.size(); ++i)
		{
			mHistory[i].setPosition(sf::Vector2f(10.f, 150.f + (i * 20.f)));
			mHistory[i].setColor(sf::Color::White);
			mHistory[i].setFont(mFont);
			mHistory[i].setCharacterSize(15);
			mHistory[i].setString("Alex: Hallo wie geht es dir in diesem Spiel");
		}
	}

	void ChatHandler::updateVisual()
	{
		for (std::size_t i = 0; i < mHistory.size(); ++i)
		{
			std::size_t positionInDeque = GetOutputBuffer().size() - 1 - i;

			if (positionInDeque < GetOutputBuffer().size())
			{
				mHistory[mHistory.size() - 1 - i].setString(GetOutputBuffer()[positionInDeque].text);
				mHistory[mHistory.size() - 1 - i].setColor(sf::Color::White);
				//mHistory[mHistory.size() - 1 - i].setColor(GetOutputBuffer()[positionInDeque].colour);
			}
			else
			{
				mHistory[mHistory.size() - 1 - i].setString("");
			}
		}

		mInputText.setString(GetInputText());
	}

	void ChatHandler::receiveMessage(const Message &msg)
	{
		if (msg.ID == aw::hash("connect"))
		{
			std::string name = *msg.getValue<std::string>(0);
			SetUserName(name);
			//Connect(sf::IpAddress("127.0.0.1"), 12122);
			Connect(sf::IpAddress("82.211.56.205"), 12122);
		}
		else if (msg.ID == aw::hash("close game"))
		{
			Disconnect();
		}
		else if (msg.ID == aw::hash("show chat"))
		{
			mVisible = true;
			EnableInput();
		}
		else if (msg.ID == aw::hash("event") && mVisible)
		{
			sf::Event event = *msg.getValue<sf::Event>(0);
			Update(event);
			
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				mVisible = false;
				Message msg(aw::hash("hide chat"));
				m_messageBus.sendMessage(msg);
			}
		}
	}

	void ChatHandler::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if (!mVisible)
			return;

		target.draw(mSourundingRect);
		target.draw(mInputRect);
		target.draw(mInputText);
		for (auto &it : mHistory)
			target.draw(it);
	}
}