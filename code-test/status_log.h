#pragma once
#include <map>
#include <string>
#include <iostream>
#include <time.h>

class StatusLog {

private:
	std::map<std::string, std::tuple<std::string, double>> logbook;
	time_t timer;

public:
	StatusLog() {

	}
	void update_status(const std::string& machine_id, const std::string& status);
	double last_seen(const std::string& machine_id);
	std::string get_status(const std::string& machine_id);
	std::map<std::string, std::tuple<std::string, double>>& get_log();
	bool is_empty();
	void add_entry(const std::string& machine_id);
};