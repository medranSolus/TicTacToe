#include "Utils.h"
#include <boost\algorithm\string.hpp>

namespace Utils
{
	std::deque<std::string> SplitString(const std::string& input, const std::string& delimeter)
	{
		std::deque<std::string> output;
		boost::split(output, input, boost::is_any_of(delimeter));
		return output;
	}
}