add_executable(
    unit_tests
    testcase.cc
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
