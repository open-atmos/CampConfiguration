#include <gtest/gtest.h>

#include <open_atmos/mechanism_configuration/parser.hpp>

using namespace open_atmos::mechanism_configuration;

TEST(Parser, CanParseValidSpecies)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/species/valid_species") + extension);

    EXPECT_EQ(status, ConfigParseStatus::Success);
    EXPECT_EQ(mechanism.species.size(), 3);

    EXPECT_EQ(mechanism.species[0].name, "A");
    EXPECT_EQ(mechanism.species[0].unknown_properties.size(), 1);
    EXPECT_EQ(mechanism.species[0].unknown_properties["__absolute tolerance"], "\"1.0e-30\"");

    EXPECT_EQ(mechanism.species[1].name, "H2O2");
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties.size(), 6);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["HLC(298K) [mol m-3 Pa-1]"], 1.011596348);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["HLC exponential factor [K]"], 6340);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["diffusion coefficient [m2 s-1]"], 1.46e-05);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["N star"], 1.74);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["molecular weight [kg mol-1]"], 0.0340147);
    EXPECT_EQ(mechanism.species[1].optional_numerical_properties["density [kg m-3]"], 1000.0);
    EXPECT_EQ(mechanism.species[1].unknown_properties.size(), 1);
    EXPECT_EQ(mechanism.species[1].unknown_properties["__absolute tolerance"], "\"1.0e-10\"");

    EXPECT_EQ(mechanism.species[2].name, "aerosol stuff");
    EXPECT_EQ(mechanism.species[2].optional_numerical_properties.size(), 2);
    EXPECT_EQ(mechanism.species[2].optional_numerical_properties["molecular weight [kg mol-1]"], 0.5);
    EXPECT_EQ(mechanism.species[2].optional_numerical_properties["density [kg m-3]"], 1000.0);
    EXPECT_EQ(mechanism.species[2].unknown_properties.size(), 1);
    EXPECT_EQ(mechanism.species[2].unknown_properties["__absolute tolerance"], "\"1.0e-20\"");
  }
}

TEST(Parser, DetectsDuplicateSpecies)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/species/duplicate_species") + extension);

    EXPECT_EQ(status, ConfigParseStatus::DuplicateSpeciesDetected);
  }
}

TEST(Parser, DetectsMissingRequiredKeys)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/species/missing_required_key") + extension);

    EXPECT_EQ(status, ConfigParseStatus::RequiredKeyNotFound);
  }
}

TEST(Parser, DetectsInvalidKeys)
{
  Parser parser;
  std::vector<std::string> extensions = { ".json", ".yaml" };
  for (auto& extension : extensions)
  {
    auto [status, mechanism] = parser.Parse(std::string("unit_configs/species/invalid_key") + extension);

    EXPECT_EQ(status, ConfigParseStatus::InvalidKey);
  }
}