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

wxString HashKey::GenerateString() const
{
	wxString text;
	text.reserve((1 + 2 * 4) * 4);
	for (int i = 0; i < 8; ++i)
	{
		text += wxString::Format(wxT("%02X%02X%02X%02X-"),
			key[i * 4 + 0], key[i * 4 + 1], key[i * 4 + 2], key[i * 4 + 3]);
	}
	text.erase(text.length() - 1);
	return text;
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