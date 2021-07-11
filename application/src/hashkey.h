#pragma once
#include<array>
#include<xhash>
#include<wx/string.h>
class HashKey
{
	friend struct std::hash<HashKey>;
public:
	HashKey(std::array<uint8_t, 32> const& key);
	wxString GenerateString()const;
	uint8_t operator[](size_t index) const;
	bool operator==(const HashKey& rhs)const;
	bool operator < (const HashKey& rhs)const;
private:
	std::array<uint8_t, 32> key;
	size_t hash;
};

namespace std
{
	template<>
	struct hash<HashKey>
	{
		size_t operator()(const HashKey& obj)const;
	};
}