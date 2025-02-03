#include <mechanism_configuration/constants.hpp>
#include <mechanism_configuration/v1/parser.hpp>
#include <mechanism_configuration/v1/parser_types.hpp>
#include <mechanism_configuration/v1/utils.hpp>
#include <mechanism_configuration/validate_schema.hpp>

namespace mechanism_configuration
{
  namespace v1
  {
    Errors FirstOrderLossParser::parse(
        const YAML::Node& object,
        const std::vector<types::Species>& existing_species,
        const std::vector<types::Phase>& existing_phases,
        types::Reactions& reactions)
    {
      Errors errors;
      types::FirstOrderLoss first_order_loss;

      auto validate = ValidateSchema(object, validation::first_order_loss.required_keys, validation::first_order_loss.optional_keys);
      errors.insert(errors.end(), validate.begin(), validate.end());
      if (validate.empty())
      {
        auto reactants = ParseReactantsOrProducts(validation::keys.reactants, object);
        errors.insert(errors.end(), reactants.first.begin(), reactants.first.end());

        if (object[validation::keys.scaling_factor])
        {
          first_order_loss.scaling_factor = object[validation::keys.scaling_factor].as<double>();
        }

        if (object[validation::keys.name])
        {
          first_order_loss.name = object[validation::keys.name].as<std::string>();
        }

        std::vector<std::string> requested_species;
        for (const auto& spec : reactants.second)
        {
          requested_species.push_back(spec.species_name);
        }

        if (RequiresUnknownSpecies(requested_species, existing_species))
        {
          std::string line = std::to_string(object.Mark().line + 1);
          std::string column = std::to_string(object.Mark().column + 1);
          errors.push_back({ ConfigParseStatus::ReactionRequiresUnknownSpecies, line + ":" + column + ": Reaction requires unknown species" });
        }

        std::string gas_phase = object[validation::keys.gas_phase].as<std::string>();
        auto it = std::find_if(existing_phases.begin(), existing_phases.end(), [&gas_phase](const auto& phase) { return phase.name == gas_phase; });
        if (it == existing_phases.end())
        {
          std::string line = std::to_string(object[validation::keys.gas_phase].Mark().line + 1);
          std::string column = std::to_string(object[validation::keys.gas_phase].Mark().column + 1);
          errors.push_back({ ConfigParseStatus::UnknownPhase, line + ":" + column + ": Unknown phase: " + gas_phase });
        }

        if (reactants.second.size() > 1)
        {
          std::string line = std::to_string(object[validation::keys.reactants].Mark().line + 1);
          std::string column = std::to_string(object[validation::keys.reactants].Mark().column + 1);
          errors.push_back({ ConfigParseStatus::TooManyReactionComponents, line + ":" + column + ": Too many reaction components" });
        }

        first_order_loss.gas_phase = gas_phase;
        first_order_loss.reactants = reactants.second;
        first_order_loss.unknown_properties =
            GetComments(object, validation::first_order_loss.required_keys, validation::first_order_loss.optional_keys);
        reactions.first_order_loss.push_back(first_order_loss);
      }

      return errors;;
    }
  }  // namespace v1
}  // namespace mechanism_configuration
