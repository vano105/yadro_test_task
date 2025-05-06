#pragma once

#include <string>

bool is_valid_client_name(const std::string &name);
void throw_formatted_error(int line_num, const std::string &message);
