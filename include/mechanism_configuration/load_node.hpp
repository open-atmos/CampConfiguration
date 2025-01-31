#pragma once

#include <yaml-cpp/yaml.h>

#include <mechanism_configuration/mechanism.hpp>
#include <string>

namespace mechanism_configuration
{
  // Helper for static_assert to provide meaningful error messages
  template<typename>
  struct always_false : std::false_type
  {
  };

  template <typename T>
  constexpr bool IsStringOrPath()
  {
      return std::is_same_v<std::decay_t<T>, std::string> || std::is_same_v<std::decay_t<T>, std::filesystem::path>;
  }

  /// @brief Helper to load YAML node from a file path, string, or YAML::Node
  /// @param source A file path, string, or YAML::Node
  /// @return A YAML node
  template<typename T>
  YAML::Node LoadNode(const T& source)
  {
    if constexpr (std::is_same_v<std::decay_t<T>, YAML::Node>)
    {
      return source;
    }
    else if constexpr (IsStringOrPath<T>())
    {
      // check if the file exists
      if (std::filesystem::exists(source))
      {
        return YAML::LoadFile(source);
      }
      else {
        std::cerr << "File does not exist: " << source << std::endl;
      }
      return YAML::Node();
    }
    else
    {
      static_assert(always_false<T>::value, "Unsupported type for LoadNode");
    }
  }
}  // namespace mechanism_configuration