#pragma once
#include <cstdlib>
#include "Updater.h"

template<typename KeyType, typename ValueType>
class Map {
private:
	class PutUpdater : public Updater<KeyType, ValueType> {
	public:
		PutUpdater(ValueType new_value) : new_value(new_value) {}
		virtual ~PutUpdater() {}

		virtual void updateKeyValuePair(KeyType * p_key, ValueType * p_value) {
			*p_value = new_value;
		}

		virtual bool needNewKeyValuePair(KeyType * p_key, ValueType * p_value) {
			*p_value = new_value;
			return true;
		}
	private:
		ValueType new_value;
	};

public:
	void put(KeyType key, ValueType value) {
		Updater<KeyType, ValueType> * p_updater = new PutUpdater(value);
		update(key, p_updater);
		delete p_updater;
	}

	virtual void update(KeyType key, Updater<KeyType, ValueType> * p_updater) = 0;
	virtual bool get(KeyType key, ValueType * p_value) = 0;
	virtual bool hasKey(KeyType key) = 0;
	virtual void remove(KeyType key) = 0;
	virtual size_t size() = 0;

	virtual ~Map<KeyType, ValueType>() {}
};
