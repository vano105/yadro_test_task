#include "utils.hpp"
#include <regex>

bool is_valid_client_name(const std::string &name) {
  std::regex pattern("^[a-z0-9_-]+$");
  return std::regex_match(name, pattern);
}

void throw_formatted_error(int line_num, const std::string &message) {
  throw std::runtime_error("Line " + std::to_string(line_num) + ": " + message);
}
