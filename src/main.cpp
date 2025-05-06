#include "club/club.hpp"
#include "event/event.hpp"
#include "utils/utils.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void print_events(const std::vector<club_event> &events) {
  for (const auto &e : events) {
    std::cout << e.time.to_string() << " " << e.id;
    if (!e.client_name.empty())
      std::cout << " " << e.client_name;
    if (e.table_id != 0)
      std::cout << " " << e.table_id;
    if (!e.error_msg.empty())
      std::cout << " " << e.error_msg;
    std::cout << "\n";
  }
}

void print_stats(const std::vector<table_info> &tables) {
  for (size_t i = 0; i < tables.size(); ++i) {
    std::cout << i + 1 << " " << tables[i].revenue << " "
              << tables[i].total_time.to_string() << "\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  try {
    std::ifstream file(argv[1]);
    std::string line;
    int line_num = 0;

    // Read number of tables
    std::getline(file, line);
    line_num++;
    int num_tables = std::stoi(line);
    if (num_tables <= 0)
      throw_formatted_error(line_num, "Invalid number of tables");

    // Read working hours
    std::getline(file, line);
    line_num++;
    std::istringstream iss(line);
    std::string open_str, close_str;
    if (!(iss >> open_str >> close_str))
      throw_formatted_error(line_num, "Invalid time format");
    time_utils open_time = time_utils::parse_time(open_str);
    time_utils close_time = time_utils::parse_time(close_str);

    // Read hour cost
    std::getline(file, line);
    line_num++;
    int hour_cost = std::stoi(line);
    if (hour_cost <= 0)
      throw_formatted_error(line_num, "Invalid hour cost");

    computer_club club(num_tables, open_time, close_time, hour_cost);

    // Read events
    std::vector<club_event> events;
    while (std::getline(file, line)) {
      line_num++;
      if (line.empty())
        continue;
      events.push_back(club_event::parse_event(line));
    }

    // Process events
    for (const auto &event : events) {
      club.process_event(event);
    }
    club.end_of_day();

    // Output results
    std::cout << open_time.to_string() << "\n";
    print_events(club.get_output_events());
    std::cout << close_time.to_string() << "\n";
    print_stats(club.get_tables_stats());

  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
