add_executable(
    unit_tests
    test_avltreemap.cc
    test_sbtreemap.cc
	test_rbtreemap.cc
    )

target_link_libraries(
    unit_tests
    gtest_main
    )

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../src
    )

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )
