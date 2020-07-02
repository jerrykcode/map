#include <gtest/gtest.h>
#include "map.h"
#include "avltreemap.h"
#include "tester.h"
using namespace std;

size_t avl_test_num = 100000;

TEST(AVLTreeMap, IntIntPutAndGet) {
	Tester<int, int> tester(avl_test_num, 0 - avl_test_num, avl_test_num, 0 - avl_test_num, avl_test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.TestPutAndGet(map);
	delete map;
}

TEST(AVLTreeMap, IntIntHasKey) {
	Tester<int, int> tester(avl_test_num, 0 - avl_test_num/100, avl_test_num/100, 0 - avl_test_num/100, avl_test_num/100);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.TestHasKey(map);
	delete map;
}

TEST(AVLTreeMap, IntIntRemove) {
	Tester<int, int> tester(avl_test_num, 0 - avl_test_num, avl_test_num, 0 - avl_test_num, avl_test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.TestRemove(map);
	delete map;
}

TEST(AVLTreeMap, SPEED) {
	Tester<int, int> tester(avl_test_num, 0 - avl_test_num, avl_test_num, 0 - avl_test_num, avl_test_num);
	Map<int, int> * map = new AVLTreeMap<int, int>();
	tester.TestSpeed(map, "AVLTreeMap");
	delete map;
}
