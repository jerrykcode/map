#pragma once

#include <cstdlib>

template<typename KeyType, typename ValueType>
class Updater {
public:
	virtual void UpdateKeyValuePair(KeyType * p_key, ValueType * p_value) = 0;
	virtual bool NeedNewKeyValuePair(KeyType * p_key, ValueType * p_value) = 0;
	virtual ~Updater() {}
};
