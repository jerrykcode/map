#include <gtest/gtest.h>
#include "map.h"
#include "sbtreemap.h"
#include "tester.h"
using namespace std;

size_t sbt_test_num = 100000;

TEST(SBTreeMap, IntIntPutAndGet) {
	Tester<int, int> tester(sbt_test_num, 0 - sbt_test_num, sbt_test_num, 0 - sbt_test_num, sbt_test_num);
	Map<int, int> * map = new SBTreeMap<int, int>();
	tester.TestPutAndGet(map);
	delete map;
}

TEST(SBTreeMap, IntIntHasKey) {
	Tester<int, int> tester(sbt_test_num, 0 - sbt_test_num, sbt_test_num, 0 - sbt_test_num, sbt_test_num);
	Map<int, int> * map = new SBTreeMap<int, int>();
	tester.TestHasKey(map);
	delete map;
}

TEST(SBTreeMap, IntIntRemove) {
	Tester<int, int> tester(sbt_test_num, 0 - sbt_test_num, sbt_test_num, 0 - sbt_test_num, sbt_test_num);
	Map<int, int> * map = new SBTreeMap<int, int>();
	tester.TestRemove(map);
	delete map;
}

TEST(SBTreeMap, SPEED) {
	Tester<int, int> tester(sbt_test_num, 0 - sbt_test_num, sbt_test_num, 0 - sbt_test_num, sbt_test_num);
	Map<int, int> * map = new SBTreeMap<int, int>();
	tester.TestSpeed(map, "SBTreeMap");
	delete map;
}
