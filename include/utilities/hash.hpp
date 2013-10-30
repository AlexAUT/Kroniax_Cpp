#ifndef AWHASH_HPP
#define AWHASH_HPP

namespace aw
{
	//A really simple and fast hash function
	unsigned int hash(const char* s, unsigned int seed = 0);
}

#endif