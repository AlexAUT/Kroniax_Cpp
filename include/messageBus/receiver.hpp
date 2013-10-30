#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <vector>
#include <memory>

#include "../utilities/anyType.hpp"

namespace aw
{
	struct Message
	{
		std::size_t ID;
		std::vector<std::unique_ptr<aw::BaseValue>> m_values;

		Message() {}
		Message(const std::size_t &pID) :
			ID(pID)
		{}

		void clear()
		{
			m_values.clear();
		}

		template <class T>
		void push_back(const T &value)
		{
			m_values.push_back(std::unique_ptr<BaseValue>(new Value<T>(value)));
		}

		template <class T>
		T *getValue(std::size_t index) const
		{
			if (index < m_values.size())
			{
				return &static_cast<Value<T>*>(&*m_values[index])->m_value;
			}

			return nullptr;
		}
	};

	//Interface to add a class to the MessageBus
	class Receiver
	{
	public:
		virtual void receiveMessage(const Message &msg) = 0;
	};
}

#endif