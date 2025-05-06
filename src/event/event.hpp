#pragma once

#include "../time/time.hpp"
#include <string>
#include <vector>

enum class event_type { incoming, outgoing };

struct club_event {
  time_utils time;
  int id;
  std::string client_name;
  int table_id;
  std::string error_msg;
  event_type type;

  club_event(time_utils t, int i, const std::string &name, int tid,
             const std::string &err, event_type tp);
  static club_event parse_event(const std::string &line);
};
