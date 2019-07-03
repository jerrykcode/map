#pragma once
#include <cstdlib>

template<typename KeyType, typename ValueType>
class Map {
public:
	virtual void put(KeyType key, ValueType value) = 0;
	virtual bool get(KeyType key, ValueType * p_value) = 0;
	virtual bool hasKey(KeyType key) = 0;
	virtual void remove(KeyType key) = 0;
	virtual size_t size() = 0;

	virtual ~Map<KeyType, ValueType>() {}
};