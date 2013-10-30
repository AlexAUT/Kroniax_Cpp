#include "hash.hpp"

namespace aw
{
	unsigned int hash(const char* s, unsigned int seed)
	{
		unsigned hash = seed;
		while (*s)
		{
			hash = hash * 101 + *s++;
		}
		return hash;
	}
}