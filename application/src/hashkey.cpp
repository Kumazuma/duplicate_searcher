#include "hashkey.h"

HashKey::HashKey(std::array<uint8_t, 32> const& key):key{key}
{
	size_t ret{};
	const size_t* it = reinterpret_cast<const size_t*>(&key[0]);
	for (int i = 0; i < 32 / sizeof(size_t); ++i)
	{
		ret ^= it[i];
	}
	hash = ret;
}

uint8_t HashKey::operator[](size_t index) const
{
	return key[index];
}

bool HashKey::operator==(const HashKey& rhs) const
{
	return memcmp(rhs.key.data(), key.data(), 32) == 0;
}

bool HashKey::operator<(const HashKey& rhs) const
{
	return memcmp(key.data() , rhs.key.data(), 32) < 0;
}

namespace std
{

	size_t hash<HashKey>::operator()(const HashKey& obj) const
	{

		return obj.hash;
	}
}