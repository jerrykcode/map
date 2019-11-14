#pragma once
#include <gtest/gtest.h>
#include "Map.h"
#include "AVLTreeMap.h"
#include <map>
#include <iterator>
#include <iostream>
#include <vector>
#include "Random.h"
#include "time.h"
using namespace std;

template<typename KeyType, typename ValueType>
class Tester {
public:
	Tester(size_t test_num, KeyType key_min, KeyType key_max, ValueType value_min, ValueType value_max) : 
		test_num(test_num), 
		key_min(key_min), key_max(key_max),
		value_min(value_min), value_max(value_max) {}

	void testPutAndGet(Map<KeyType, ValueType> * testing_map);
	void testHasKey(Map<KeyType, ValueType>* testing_map);
	void testRemove(Map<KeyType, ValueType> * testing_map);
	void testSpeed(Map<KeyType, ValueType> * testing_map, string testing_map_name);
private:
	size_t test_num;
	KeyType key_min, key_max;
	ValueType value_min, value_max;
};

template<typename KeyType, typename ValueType>
inline void Tester<KeyType, ValueType>::testPutAndGet(Map<KeyType, ValueType>* testing_map) {
	map<KeyType, ValueType> std_map; //std::map
	size_t i;
	//Random for key and value
	Random<KeyType> key_random(key_min, key_max);
	Random<ValueType> value_random(value_min, value_max);
	//Put <key, value> pairs into the maps
	for (i = 0; i < test_num; i++) {
		KeyType key = key_random.rand();
		ValueType value = value_random.rand();
		//cout << "key: " << key << "----------value: " << value << endl;
		testing_map->put(key, value);
		std_map[key] = value;
	}
	EXPECT_EQ(testing_map->size(), std_map.size());
	//Get value by key from the maps
	size_t get_num = 0;
	while (get_num < testing_map->size() / 2) {
		KeyType key = key_random.rand();
		ValueType value;
		if (testing_map->get(key, &value)) { //key exists in the map
			EXPECT_EQ(value, std_map[key]);
			get_num++;
		}
		else { //key not exists
			EXPECT_EQ(0, std_map[key]);
		}
	}	
	map<KeyType, ValueType>().swap(std_map);
}

template<typename KeyType, typename ValueType>
inline void Tester<KeyType, ValueType>::testHasKey(Map<KeyType, ValueType>* testing_map) {
	map<KeyType, ValueType> std_map; //std::map
	//Random for key and value
	Random<KeyType> key_random(key_min, key_max);
	Random<ValueType> value_random(value_min, value_max);
	//Test 'hasKey'
	for (size_t i = 0; i < test_num; i++) {
		KeyType key = key_random.rand();		
		if (std_map.find(key) != std_map.end()) { //has key
			ASSERT_TRUE(testing_map->hasKey(key));
		}
		else {
			ASSERT_FALSE(testing_map->hasKey(key));
			//insert
			ValueType value = value_random.rand();			
			testing_map->put(key, value);
			std_map[key] = value;
		}
	}
	EXPECT_EQ(testing_map->size(), std_map.size());
	map<KeyType, ValueType>().swap(std_map);
}

template<typename KeyType, typename ValueType>
inline void Tester<KeyType, ValueType>::testRemove(Map<KeyType, ValueType>* testing_map) {
	map<KeyType, ValueType> std_map; //std::map
	//Random for key and value
	Random<KeyType> key_random(key_min, key_max);
	Random<ValueType> value_random(value_min, value_max);
	//Put <key, value> pairs
	KeyType key;
	ValueType value;
	for (size_t i = 0; i < test_num; i++) {
		key = key_random.rand();
		value = value_random.rand();
		testing_map->put(key, value);
		std_map[key] = value;
	}
	EXPECT_EQ(testing_map->size(), std_map.size());
	size_t size = testing_map->size();
	vector<KeyType> removed_keys;
	typename map<KeyType, ValueType>::iterator it;
	while (testing_map->size() > size / 2) { //Remove half of the <key, value> pairs
		key = key_random.rand();
		testing_map->remove(key); //If key dose not exists, Map<KeyType, ValueType>::remove(KeyType key) function will not remove any <key, value> pair
		it = std_map.find(key);		
		if (it != std_map.end()) std_map.erase(it);		
		removed_keys.push_back(key);
		EXPECT_EQ(testing_map->size(), std_map.size());
	}
	EXPECT_EQ(testing_map->size(), std_map.size());
	for (KeyType key : removed_keys) {
		ASSERT_FALSE(testing_map->hasKey(key));
	}
	//Get value by key from the maps
	size_t get_num = 0;
	while (get_num < testing_map->size() / 2) {
		KeyType key = key_random.rand();
		ValueType value;
		if (testing_map->get(key, &value)) { //key exists in the map
			EXPECT_EQ(value, std_map[key]);
			get_num++;
		}
		else { //key not exists
			EXPECT_EQ(0, std_map[key]);
		}
	}
	map<KeyType, ValueType>().swap(std_map);
	vector<KeyType>().swap(removed_keys);
}

template<typename KeyType, typename ValueType>
inline void Tester<KeyType, ValueType>::testSpeed(Map<KeyType, ValueType>* testing_map, string testing_map_name) {
	int * key_set = new int[test_num];
	int * value_set = new int[test_num];
	//Random for key and value
	Random<KeyType> key_random(key_min, key_max);
	Random<ValueType> value_random(value_min, value_max);
	size_t i = 0;
	for (i = 0; i < test_num; i++) {
		key_set[i] = key_random.rand();
		value_set[i] = key_random.rand();
	}
		
	map<int, int> std_map;

	clock_t start, end;
	//Put	
	//Testing map
	start = clock();
	for (i = 0; i < test_num; i++) {
		testing_map->put(key_set[i], value_set[i]);
	}
	end = clock();
	cout << "Put random <key, value> pair : " << testing_map_name << " cost " << (end - start) << "ms" << endl;

	//std::map
	start = clock();
	for (i = 0; i < test_num; i++) {
		std_map[key_set[i]] = value_set[i];
	}
	end = clock();
	cout << "Insert random <key, value> pair : std::map cost " << (end - start) << "ms" << endl;

	//Get	
	//Testing map
	start = clock();
	for (i = 0; i < test_num; i++) {
		int value;
		testing_map->get(key_set[i], &value);
	}
	end = clock();
	cout << "Get value by key : " << testing_map_name << " cost " << (end - start) << "ms" << endl;

	//std::map
	start = clock();
	for (i = 0; i < test_num; i++) {
		int value = std_map[key_set[i]];
	}
	end = clock();
	cout << "Get value by key : std::map cost " << (end - start) << "ms" << endl;

	//Remove
	//Testing map
	start = clock();
	for (i = 0; i < test_num; i++) {
		testing_map->remove(key_set[i]);
	}
	end = clock();
	cout << "Remove all the <key, value> pairs : " << testing_map_name << " cost " << (end - start) << "ms" << endl;

	//std::map
	start = clock();
	map<int, int>::iterator it;
	for (i = 0; i < test_num; i++) {
		it = std_map.find(key_set[i]);
		if (it != std_map.end()) {
			std_map.erase(it);
		}
	}
	end = clock();
	cout << "Remove all the <key, value> pairs : std::map cost " << (end - start) << "ms" << endl;

	//Put increasing key
	//Testing map
	start = clock();
	for (i = 0; i < test_num; i++) {
		testing_map->put(i, value_set[i]); //i as key
	}
	end = clock();
	cout << "Put increasing key : " << testing_map_name << " cost " << (end - start) << "ms" << endl;

	//std::map
	start = clock();
	for (i = 0; i < test_num; i++) {
		std_map[i] = value_set[i]; //i as key
	}
	end = clock();
	cout << "Insert increasing key : std::map cost " << (end - start) << "ms" << endl;

	free(key_set);
	free(value_set);
	map<KeyType, ValueType>().swap(std_map);
}
