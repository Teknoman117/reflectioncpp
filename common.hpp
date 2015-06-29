#ifndef __REFLECTIONCPP_COMMON_HPP__
#define __REFLECTIONCPP_COMMON_HPP__

#include <string>
#include <cinttypes>

namespace reflectioncpp
{
	namespace internal
	{
		// Constant strlen
		constexpr size_t const_strlen(const char *str, const int idx = 0)
		{
			return (str[idx] == '\0') ? idx : const_strlen(str, idx+1);
		}

		// MurmurHash2 64 bit function, used for string hashing
		uint64_t hash ( const void * key, int len, size_t seed = static_cast<size_t>(0xc70f6907UL));
	}

	// A named object
	class Named
	{
		const std::string name;
	public:
		Named(const std::string name);
		const std::string& GetName() const;
	};
}

#endif