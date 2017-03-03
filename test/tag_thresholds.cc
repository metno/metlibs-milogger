
#include "miLoggingUtils.h"

#include <sstream>

#include "miLoggingUtils.h"

#include <gtest/gtest.h>

using namespace milogger;

namespace {
const char config1[] =
    "log4cpp.rootCategory=INFO, someAppender\n"
    "log4cpp.category.some.deeply.nested.tag=DEBUG\n"
    "log4cpp.category.some.deeply=FATAL, fishy\n";
} // namespace

TEST(TestParseThresholds, Basic)
{
  Severity root;
  tag_threshold_m tags;

  std::istringstream config(config1);
  parseTagThresholds(root, tags, config);

  EXPECT_EQ(milogger::INFO, root);

  const tag_threshold_m::const_iterator it1 = tags.find("some.deeply.nested.tag");
  ASSERT_TRUE(it1 != tags.end());
  EXPECT_EQ(milogger::DEBUG, it1->second);

  ASSERT_TRUE(tags.find("some.deeply.nested") == tags.end());

  const tag_threshold_m::const_iterator it2 = tags.find("some.deeply");
  ASSERT_TRUE(it2 != tags.end());
  EXPECT_EQ(milogger::FATAL, it2->second);
}

TEST(TestFindThresholds, Basic)
{
  Severity root;
  tag_threshold_m tags;

  std::istringstream config(config1);
  parseTagThresholds(root, tags, config);

  EXPECT_EQ(milogger::DEBUG, findTagThreshold(root, tags, "some.deeply.nested.tag"));
  EXPECT_EQ(milogger::FATAL, findTagThreshold(root, tags, "some.deeply.nested"));
  EXPECT_EQ(milogger::FATAL, findTagThreshold(root, tags, "some.deeply"));
  EXPECT_EQ(milogger::INFO,  findTagThreshold(root, tags, "some"));
}
