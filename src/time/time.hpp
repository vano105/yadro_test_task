#pragma once

#include <stdexcept>
#include <string>

class time_utils {
public:
  int hours;
  int minutes;

  time_utils(int h = 0, int m = 0);
  static time_utils parse_time(const std::string &time_str);
  std::string to_string() const;
  int to_minutes() const;
  bool operator<(const time_utils &other) const;
  bool operator<=(const time_utils &other) const;
  bool operator>(const time_utils &other) const;
  bool operator>=(const time_utils &other) const;
  time_utils operator+(const time_utils &other) const;
  time_utils operator-(const time_utils &other) const;
};
