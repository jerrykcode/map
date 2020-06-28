#pragma once
#include <cstdlib>
#include "updater.h"

template<typename KeyType, typename ValueType>
class Map {
private:
	class PutUpdater : public Updater<KeyType, ValueType> {
	public:
		PutUpdater(ValueType new_value) : new_value(new_value) {}
		virtual ~PutUpdater() {}

		virtual void UpdateKeyValuePair(KeyType * p_key, ValueType * p_value) {
			*p_value = new_value;
		}

		virtual bool NeedNewKeyValuePair(KeyType * p_key, ValueType * p_value) {
			*p_value = new_value;
			return true;
		}
	private:
		ValueType new_value;
	};

public:
	void Put(KeyType key, ValueType value) {
		Updater<KeyType, ValueType> * p_updater = new PutUpdater(value);
		Update(key, p_updater);
		delete p_updater;
	}

	virtual void Update(KeyType key, Updater<KeyType, ValueType> * p_updater) = 0;
	virtual bool Get(KeyType key, ValueType * p_value) = 0;
	virtual bool HasKey(KeyType key) = 0;
	virtual void Remove(KeyType key) = 0;
	virtual size_t Size() = 0;

	virtual ~Map<KeyType, ValueType>() {}
};
