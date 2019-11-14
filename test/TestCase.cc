#include <gtest/gtest.h>
#include "Map.h"
#include "AVLTreeMap.h"
#include "Tester.h"
using namespace std;

size_t test_num = 100000;

TEST(AVLTreeMap, IntIntPutAndGet) {
	Tester<int, int> tester(test_num, 0 - test_num, test_num, 0 - test_num, test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.testPutAndGet(map);
	delete map;
}

TEST(AVLTreeMap, IntIntHasKey) {
	Tester<int, int> tester(test_num, 0 - test_num/100, test_num/100, 0 - test_num/100, test_num/100);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.testHasKey(map);
	delete map;
}

TEST(AVLTreeMap, IntIntRemove) {
	Tester<int, int> tester(test_num, 0 - test_num, test_num, 0 - test_num, test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.testRemove(map);
	delete map;
}

TEST(AVLTreeMap, SPEED) {
	Tester<int, int> tester(test_num, 0 - test_num, test_num, 0 - test_num, test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.testSpeed(map, "AVLTreeMap");
	delete map;
}