#pragma once

#include <cstdlib>

template<typename KeyType, typename ValueType>
class Updater {
public:
	virtual void updateKeyValuePair(KeyType * p_key, ValueType * p_value) = 0;
	virtual bool needNewKeyValuePair(KeyType * p_key, ValueType * p_value) = 0;
	virtual ~Updater() {}
};
