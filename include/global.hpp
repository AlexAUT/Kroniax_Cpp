#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <string>

#ifndef DATADIR
#define DATADIR "./data"
#endif // DATADIR

namespace Helper
{
	inline std::string getData(const std::string& filePath)
	{
		return std::string(DATADIR) + "/" + filePath;
	}
}

#endif // GLOBAL_HPP
