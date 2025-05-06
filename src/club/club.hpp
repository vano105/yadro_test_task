#pragma once

#include "../event/event.hpp"
#include "../time/time.hpp"
#include <algorithm>
#include <map>
#include <queue>
#include <vector>

struct table_info {
  bool is_occupied = false;
  time_utils occupied_since;
  int revenue = 0;
  time_utils total_time;
};

class computer_club {
  int num_tables_;
  time_utils open_time_;
  time_utils close_time_;
  int hour_cost_;
  std::vector<table_info> tables_;
  std::map<std::string, int> clients_;
  std::queue<std::string> waiting_queue_;
  std::vector<club_event> output_events_;

  void add_revenue(int table_id, const time_utils &start,
                   const time_utils &end);
  void process_id1(const club_event &event);
  void process_id2(const club_event &event);
  void process_id3(const club_event &event);
  void process_id4(const club_event &event);

public:
  computer_club(int tables_num, time_utils open, time_utils close, int cost);
  void process_event(const club_event &event);
  void end_of_day();
  std::vector<club_event> get_output_events() const;
  std::vector<table_info> get_tables_stats() const;
};
