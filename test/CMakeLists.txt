
METNO_FIND_GTEST()

INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/src"
)

ADD_EXECUTABLE(milogger_test
  log_something.cc
  tag_thresholds.cc
  test_main.cc
)

TARGET_LINK_LIBRARIES(milogger_test
  milogger
  ${GTEST_LIBRARY}
)

ADD_TEST(NAME milogger_test
  COMMAND milogger_test --gtest_color=yes
)
