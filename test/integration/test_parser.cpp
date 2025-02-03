#include <gtest/gtest.h>

#include <mechanism_configuration/parser.hpp>
#include <mechanism_configuration/v1/types.hpp>

TEST(ParserBase, ParsesFullV0Configuration)
{
  mechanism_configuration::UniversalParser parser;
  std::vector<std::string> extensions = { "yaml", "json" };
  for (auto& extension : extensions)
  {
    std::string path = "examples/v0/" + extension;
    auto parsed = parser.Parse(path);
    EXPECT_TRUE(parsed);
  }
}

TEST(ParserBase, ParsesFullV1Configuration)
{
  mechanism_configuration::UniversalParser parser;
  std::vector<std::string> extensions = { ".yaml", ".json" };
  for (auto& extension : extensions)
  {
    std::string path = "examples/v1/full_configuration" + extension;
    auto parsed = parser.Parse(path);
    EXPECT_TRUE(parsed);
  }
}

TEST(ParserBase, ParserReportsBadFiles)
{
  mechanism_configuration::UniversalParser parser;
  std::vector<std::string> extensions = { ".yaml", ".json" };
  for (auto& extension : extensions)
  {
    std::string path = "examples/_missing_configuration" + extension;
    auto parsed = parser.Parse(path);
    EXPECT_FALSE(parsed);
    EXPECT_EQ(parsed.errors.size(), 1);
    EXPECT_EQ(parsed.errors[0].first, mechanism_configuration::ConfigParseStatus::FileNotFound);
  }
}