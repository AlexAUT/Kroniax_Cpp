#ifndef ANYTYPE_HPP
#define ANYTYPE_HPP

namespace aw
{
	class BaseValue {};

	template<class T>
	class Value : public BaseValue
	{
	public:

		Value(T value):
			m_value(value)
		{}

		T m_value;
	};

}

#endif