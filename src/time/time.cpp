#include "time.hpp"
#include <iomanip>
#include <sstream>

time_utils::time_utils(int h, int m) : hours(h), minutes(m) {}

time_utils time_utils::parse_time(const std::string &time_str) {
  std::istringstream iss(time_str);
  char colon;
  int h, m;
  if (!(iss >> h >> colon >> m) || colon != ':' || h < 0 || h >= 24 || m < 0 ||
      m >= 60) {
    throw std::invalid_argument("Invalid time format");
  }
  return time_utils(h, m);
}

std::string time_utils::to_string() const {
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2)
      << std::setfill('0') << minutes;
  return oss.str();
}

int time_utils::to_minutes() const { return hours * 60 + minutes; }

bool time_utils::operator<(const time_utils &other) const {
  return to_minutes() < other.to_minutes();
}

bool time_utils::operator<=(const time_utils &other) const {
  return to_minutes() <= other.to_minutes();
}

bool time_utils::operator>(const time_utils &other) const {
  return to_minutes() > other.to_minutes();
}

bool time_utils::operator>=(const time_utils &other) const {
  return to_minutes() >= other.to_minutes();
}

time_utils time_utils::operator+(const time_utils &other) const {
  int total = to_minutes() + other.to_minutes();
  return time_utils(total / 60 % 24, total % 60);
}

time_utils time_utils::operator-(const time_utils &other) const {
  int diff = to_minutes() - other.to_minutes();
  if (diff < 0)
    diff += 24 * 60;
  return time_utils(diff / 60, diff % 60);
}
