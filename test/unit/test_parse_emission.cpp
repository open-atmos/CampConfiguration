#include <gtest/gtest.h>

#include <open_atmos/mechanism_configuration/parser.hpp>

using namespace open_atmos::mechanism_configuration;

TEST(Parser, CanParseValidEmissionReaction)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/reactions/emission/valid") + extension);
    EXPECT_EQ(status, ConfigParseStatus::Success);

    EXPECT_EQ(mechanism.reactions.emission.size(), 2);

    EXPECT_EQ(mechanism.reactions.emission[0].gas_phase, "gas");
    EXPECT_EQ(mechanism.reactions.emission[0].name, "my emission");
    EXPECT_EQ(mechanism.reactions.emission[0].scaling_factor, 12.3);
    EXPECT_EQ(mechanism.reactions.emission[0].products.size(), 1);
    EXPECT_EQ(mechanism.reactions.emission[0].products[0].species_name, "B");
    EXPECT_EQ(mechanism.reactions.emission[0].products[0].coefficient, 1);
    EXPECT_EQ(mechanism.reactions.emission[0].unknown_properties.size(), 1);
    EXPECT_EQ(mechanism.reactions.emission[0].unknown_properties["__comment"], "\"Dr. Pepper outranks any other soda\"");

    EXPECT_EQ(mechanism.reactions.emission[1].gas_phase, "gas");
    EXPECT_EQ(mechanism.reactions.emission[1].scaling_factor, 1);
    EXPECT_EQ(mechanism.reactions.emission[1].products.size(), 1);
    EXPECT_EQ(mechanism.reactions.emission[1].products[0].species_name, "B");
    EXPECT_EQ(mechanism.reactions.emission[1].products[0].coefficient, 1);
  }
}

TEST(Parser, EmissionDetectsUnknownSpecies)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/reactions/emission/unknown_species") + extension);
    EXPECT_EQ(status, ConfigParseStatus::ReactionRequiresUnknownSpecies);
  }
}

TEST(Parser, EmissionDetectsBadReactionComponent)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/reactions/emission/bad_reaction_component") + extension);
    EXPECT_EQ(status, ConfigParseStatus::RequiredKeyNotFound);
  }
}

TEST(Parser, EmissionDetectsUnknownPhase)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/reactions/emission/missing_phase") + extension);
    EXPECT_EQ(status, ConfigParseStatus::UnknownPhase);
  }
}