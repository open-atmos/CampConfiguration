#include <gtest/gtest.h>

#include <mechanism_configuration/v1/parser.hpp>

using namespace mechanism_configuration;

TEST(Parser, CanParseValidFirstOrderLossReaction)
{
  v1::Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto parsed = parser.Parse(std::string("unit_configs/reactions/first_order_loss/valid") + extension);
    EXPECT_TRUE(parsed);
    v1::types::Mechanism mechanism = *parsed;

    EXPECT_EQ(mechanism.reactions.first_order_loss.size(), 2);

    EXPECT_EQ(mechanism.reactions.first_order_loss[0].gas_phase, "gas");
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].name, "my first order loss");
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].scaling_factor, 12.3);
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].reactants.size(), 1);
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].reactants[0].species_name, "C");
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].reactants[0].coefficient, 1);
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].unknown_properties.size(), 1);
    EXPECT_EQ(mechanism.reactions.first_order_loss[0].unknown_properties["__comment"], "\"Strawberries are the superior fruit\"");

    EXPECT_EQ(mechanism.reactions.first_order_loss[1].gas_phase, "gas");
    EXPECT_EQ(mechanism.reactions.first_order_loss[1].scaling_factor, 1);
    EXPECT_EQ(mechanism.reactions.first_order_loss[1].reactants.size(), 1);
    EXPECT_EQ(mechanism.reactions.first_order_loss[1].reactants[0].species_name, "C");
    EXPECT_EQ(mechanism.reactions.first_order_loss[1].reactants[0].coefficient, 1);
  }
}

TEST(Parser, FirstOrderLossDetectsUnknownSpecies)
{
  v1::Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto parsed = parser.Parse(std::string("unit_configs/reactions/first_order_loss/unknown_species") + extension);
    EXPECT_FALSE(parsed);
  }
}

TEST(Parser, FirstOrderLossDetectsBadReactionComponent)
{
  v1::Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto parsed = parser.Parse(std::string("unit_configs/reactions/first_order_loss/bad_reaction_component") + extension);
    EXPECT_FALSE(parsed);
  }
}

TEST(Parser, FirstOrderLossDetectsUnknownPhase)
{
  v1::Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto parsed = parser.Parse(std::string("unit_configs/reactions/first_order_loss/missing_phase") + extension);
    EXPECT_FALSE(parsed);
  }
}

TEST(Parser, FirstOrderLossDetectsMoreThanOneSpecies)
{
  v1::Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto parsed = parser.Parse(std::string("unit_configs/reactions/first_order_loss/too_many_reactants") + extension);
    EXPECT_FALSE(parsed);
  }
}