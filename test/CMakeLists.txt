cmake_minimum_required(VERSION 2.8)
project(TestMap)

include_directories(../)
include_directories(../TreeMap)

#gtest
include_directories(E:/gitcode/download/googletest-release-1.8.1/googletest/include)
include_directories(E:/gitcode/download/googletest-release-1.8.1/googletest/include/gtest/internal)

link_libraries(E:/gitcode/download/googletest-release-1.8.1/googletest/msvc/2010/gtest/Win32-Debug/gtestd.lib)

add_executable(TestMap 
			Main.cpp
			TestCase.cpp)