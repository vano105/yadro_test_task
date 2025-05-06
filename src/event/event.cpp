#include "event.hpp"
#include "../utils/utils.hpp"
#include <sstream>
#include <vector>

club_event::club_event(time_utils t, int i, const std::string &name, int tid,
                       const std::string &err, event_type tp)
    : time(t), id(i), client_name(name), table_id(tid), error_msg(err),
      type(tp) {}

club_event club_event::parse_event(const std::string &line) {
  std::istringstream iss(line);
  std::vector<std::string> parts;
  std::string part;
  while (iss >> part)
    parts.push_back(part);

  if (parts.size() < 2)
    throw std::invalid_argument("Invalid event format");

  time_utils time = time_utils::parse_time(parts[0]);
  int id = std::stoi(parts[1]);
  std::string client_name, error_msg;
  int table_id = 0;
  event_type type = event_type::incoming;

  switch (id) {
  case 1:
    if (parts.size() != 3 || !is_valid_client_name(parts[2]))
      throw std::invalid_argument("Invalid event 1 format");
    client_name = parts[2];
    break;
  case 2:
    if (parts.size() != 4 || !is_valid_client_name(parts[2]))
      throw std::invalid_argument("Invalid event 2 format");
    client_name = parts[2];
    table_id = std::stoi(parts[3]);
    break;
  case 3:
    if (parts.size() != 3 || !is_valid_client_name(parts[2]))
      throw std::invalid_argument("Invalid event 3 format");
    client_name = parts[2];
    break;
  case 4:
    if (parts.size() != 3 || !is_valid_client_name(parts[2]))
      throw std::invalid_argument("Invalid event 4 format");
    client_name = parts[2];
    break;
  default:
    throw std::invalid_argument("Unknown event ID");
  }

  return club_event(time, id, client_name, table_id, "", type);
}
