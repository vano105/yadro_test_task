#include "club.hpp"
#include "../utils/utils.hpp"
#include <algorithm>

computer_club::computer_club(int tables_num, time_utils open, time_utils close,
                             int cost)
    : num_tables_(tables_num), open_time_(open), close_time_(close),
      hour_cost_(cost) {
  tables_.resize(tables_num + 1);
}

void computer_club::add_revenue(int table_id, const time_utils &start,
                                const time_utils &end) {
  int minutes = (end - start).to_minutes();
  int hours = (minutes + 59) / 60;
  tables_[table_id].revenue += hours * hour_cost_;
  tables_[table_id].total_time = tables_[table_id].total_time + (end - start);
}

void computer_club::process_id1(const club_event &event) {
  if (event.time < open_time_ || event.time >= close_time_) {
    output_events_.emplace_back(event.time, 13, "", 0, "NotOpenYet",
                                event_type::outgoing);
    return;
  }
  if (clients_.count(event.client_name)) {
    output_events_.emplace_back(event.time, 13, "", 0, "YouShallNotPass",
                                event_type::outgoing);
    return;
  }
  clients_[event.client_name] = -1;
}

void computer_club::process_id2(const club_event &event) {
  if (!clients_.count(event.client_name)) {
    output_events_.emplace_back(event.time, 13, "", 0, "ClientUnknown",
                                event_type::outgoing);
    return;
  }
  if (tables_[event.table_id].is_occupied) {
    output_events_.emplace_back(event.time, 13, "", 0, "PlaceIsBusy",
                                event_type::outgoing);
    return;
  }

  int prev_table = clients_[event.client_name];
  if (prev_table != -1) {
    add_revenue(prev_table, tables_[prev_table].occupied_since, event.time);
    tables_[prev_table].is_occupied = false;
  }

  tables_[event.table_id].is_occupied = true;
  tables_[event.table_id].occupied_since = event.time;
  clients_[event.client_name] = event.table_id;
}

void computer_club::process_id3(const club_event &event) {
  if (!clients_.count(event.client_name)) {
    output_events_.emplace_back(event.time, 13, "", 0, "ClientUnknown",
                                event_type::outgoing);
    return;
  }

  for (int i = 1; i <= num_tables_; ++i) {
    if (!tables_[i].is_occupied) {
      output_events_.emplace_back(event.time, 13, "", 0, "ICanWaitNoLonger!",
                                  event_type::outgoing);
      return;
    }
  }

  if (waiting_queue_.size() >= num_tables_) {
    output_events_.emplace_back(event.time, 11, event.client_name, 0, "",
                                event_type::outgoing);
    clients_.erase(event.client_name);
  } else {
    waiting_queue_.push(event.client_name);
  }
}

void computer_club::process_id4(const club_event &event) {
  if (!clients_.count(event.client_name)) {
    output_events_.emplace_back(event.time, 13, "", 0, "ClientUnknown",
                                event_type::outgoing);
    return;
  }

  int table_id = clients_[event.client_name];
  if (table_id != -1) {
    add_revenue(table_id, tables_[table_id].occupied_since, event.time);
    tables_[table_id].is_occupied = false;

    if (!waiting_queue_.empty()) {
      std::string next_client = waiting_queue_.front();
      waiting_queue_.pop();
      clients_[next_client] = table_id;
      tables_[table_id].occupied_since = event.time;
      tables_[table_id].is_occupied = true;
      output_events_.emplace_back(event.time, 12, next_client, table_id, "",
                                  event_type::outgoing);
    }
  }
  clients_.erase(event.client_name);
}

void computer_club::process_event(const club_event &event) {
  output_events_.push_back(event);
  switch (event.id) {
  case 1:
    process_id1(event);
    break;
  case 2:
    process_id2(event);
    break;
  case 3:
    process_id3(event);
    break;
  case 4:
    process_id4(event);
    break;
  default:
    break;
  }
}

void computer_club::end_of_day() {
  std::vector<std::string> remaining_clients;
  for (const auto &[client, _] : clients_)
    remaining_clients.push_back(client);
  std::sort(remaining_clients.begin(), remaining_clients.end());

  for (const auto &client : remaining_clients) {
    output_events_.emplace_back(close_time_, 11, client, 0, "",
                                event_type::outgoing);
    int table_id = clients_[client];
    if (table_id != -1) {
      add_revenue(table_id, tables_[table_id].occupied_since, close_time_);
      tables_[table_id].is_occupied = false;
    }
  }
  clients_.clear();
}

std::vector<club_event> computer_club::get_output_events() const {
  return output_events_;
}

std::vector<table_info> computer_club::get_tables_stats() const {
  return std::vector<table_info>(tables_.begin() + 1, tables_.end());
}
