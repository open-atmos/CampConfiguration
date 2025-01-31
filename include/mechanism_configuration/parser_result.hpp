#pragma once

#include <vector>
#include <string>
#include <mechanism_configuration/mechanism.hpp>
#include <memory>
#include <utility>

namespace mechanism_configuration
{

  template<typename MechanismType = GlobalMechanism>
  struct ParserResult
  {
    std::unique_ptr<MechanismType> mechanism = nullptr;
    std::vector<std::pair<ConfigParseStatus, std::string>> errors;

    bool has_errors() const
    {
      return !errors.empty();
    }

    operator bool() const
    {
      return mechanism != nullptr && errors.empty();
    }

    MechanismType& operator*()
    {
      return *mechanism;
    }
  };
}  // namespace mechanism_configuration
