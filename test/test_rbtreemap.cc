#include <gtest/gtest.h>
#include "map.h"
#include "rbtreemap.h"
#include "tester.h"
using namespace std;

size_t rbt_test_num = 100000;

TEST(RBTreeMap, IntIntPutAndGet) {
	Tester<int, int> tester(rbt_test_num, 0 - rbt_test_num, rbt_test_num, 0 - rbt_test_num, rbt_test_num);
	Map<int, int> * map = new RBTreeMap<int, int>();
	tester.TestPutAndGet(map);
	delete map;
}

TEST(RBTreeMap, IntIntHasKey) {
	Tester<int, int> tester(rbt_test_num, 0 - rbt_test_num, rbt_test_num, 0 - rbt_test_num, rbt_test_num);
	Map<int, int> * map = new RBTreeMap<int, int>();
	tester.TestHasKey(map);
	delete map;
}

TEST(RBTreeMap, IntIntRemove) {
	Tester<int, int> tester(rbt_test_num, 0 - rbt_test_num, rbt_test_num, 0 - rbt_test_num, rbt_test_num);
	Map<int, int> * map = new RBTreeMap<int, int>();
	tester.TestRemove(map);
	delete map;
}

TEST(RBTreeMap, SPEED) {
	Tester<int, int> tester(rbt_test_num, 0 - rbt_test_num, rbt_test_num, 0 - rbt_test_num, rbt_test_num);
	Map<int, int> * map = new RBTreeMap<int, int>();
	tester.TestSpeed(map, "RBTreeMap");
	delete map;
}
