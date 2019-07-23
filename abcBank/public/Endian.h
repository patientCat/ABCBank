#pragma once

#include <string>
using namespace std;

namespace PUBLIC
{

class Endian
{
public:
	static uint16_t hostToNetwork16(uint16_t host16)
	{
		if (IsBigEndian())
			return host16;

		return Swap16(host16);
	}

	static uint32_t hostToNetwork32(uint32_t host32)
	{
		if (IsBigEndian())
			return host32;

		return Swap32(host32);
	}

	static uint64_t hostToNetwork64(uint64_t host64)
	{
		if (IsBigEndian())
			return host64;

		return Swap64(host64);
	}

	static uint16_t networkToHost16(uint16_t net16)
	{
		if (IsBigEndian())
			return net16;

		return Swap16(net16);
	}

	static uint32_t networkToHost32(uint32_t net32)
	{
		if (IsBigEndian())
			return net32;

		return Swap32(net32);
	}

	static uint64_t networkToHost64(uint64_t net64)
	{
		if (IsBigEndian())
			return net64;

		return Swap64(net64);
	}


private:
	static bool IsBigEndian()
	{
		const uint16_t n = 0x0001;
		if (*(char*)&n)
			return false;

		return true;
	}

	static uint16_t Swap16(uint16_t s)
	{
		return ((s & 0xff) << 8) | ((s >> 8) & 0xff);
	}

	static uint32_t Swap32(uint32_t l)
	{
		return l >> 24 |
			   (l & 0x00ff0000) >> 8 |
			   (l & 0x0000ff00) << 8 |
			   l << 24;
	}

	static uint64_t Swap64(uint64_t ll)
	{
		return ll >> 56|
			(ll & 0x00ff000000000000) >> 40 |
			(ll & 0x0000ff0000000000) >> 24 |
			(ll & 0x000000ff00000000) >> 8  | 
			(ll & 0x00000000ff000000) << 8  | 
			(ll & 0x0000000000ff0000) << 24 |
			(ll & 0x000000000000ff00) << 40 |
			ll << 56;
	}
};


}

