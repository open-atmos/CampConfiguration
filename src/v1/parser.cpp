// Copyright (C) 2023-2024 National Center for Atmospheric Research, University of Illinois at Urbana-Champaign
//
// SPDX-License-Identifier: Apache-2.0

#include <yaml-cpp/yaml.h>

#include <mechanism_configuration/v1/parser.hpp>
#include <mechanism_configuration/v1/validation.hpp>
#include <mechanism_configuration/v1/utils.hpp>
#include <mechanism_configuration/validate_schema.hpp>

namespace mechanism_configuration
{
  namespace v1
  {
    ParserResult<types::Mechanism> Parser::Parse(const std::filesystem::path& config_path)
    {
      ParserResult<types::Mechanism> result;
      if (!std::filesystem::exists(config_path))
      {
        result.errors.push_back({ ConfigParseStatus::FileNotFound, "File not found" });
        return result;
      }
      YAML::Node object = YAML::LoadFile(config_path);
      std::unique_ptr<types::Mechanism> mechanism = std::make_unique<types::Mechanism>();

      auto validate = ValidateSchema(object, validation::mechanism.required_keys, validation::mechanism.optional_keys);

      if (!validate.empty())
      {
        result.errors = validate;
        return result;
      }

      Version version = Version(object[validation::keys.version].as<std::string>());

      if (version.major != 1)
      {
        result.errors.push_back({ ConfigParseStatus::InvalidVersion, "Invalid version." });
      }

      std::string name = object[validation::keys.name].as<std::string>();
      mechanism->name = name;

      auto species_parsing = ParseSpecies(object[validation::keys.species]);
      result.errors.insert(result.errors.end(), species_parsing.first.begin(), species_parsing.first.end());

      auto phases_parsing = ParsePhases(object[validation::keys.phases], species_parsing.second);
      result.errors.insert(result.errors.end(), phases_parsing.first.begin(), phases_parsing.first.end());

      auto reactions_parsing = ParseReactions(object[validation::keys.reactions], species_parsing.second, phases_parsing.second);
      result.errors.insert(result.errors.end(), reactions_parsing.first.begin(), reactions_parsing.first.end());

      mechanism->species = species_parsing.second;
      mechanism->phases = phases_parsing.second;
      mechanism->reactions = reactions_parsing.second;

      result.mechanism = std::move(mechanism);
      return result;
    }
  }  // namespace v1
}  // namespace mechanism_configuration