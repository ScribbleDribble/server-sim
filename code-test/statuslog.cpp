#include "status_log.h"



void StatusLog::update_status(const std::string& machine_id, const std::string& status) {
	if (logbook.count(machine_id) > 0)
		logbook[machine_id] = std::make_tuple(status, time(&timer));
	return;
}

double StatusLog::last_seen(const std::string& machine_id) {
	return time(&timer) - std::get<1>(logbook[machine_id]);
}

std::string StatusLog::get_status(const std::string& machine_id) {
	return std::get<0>(logbook[machine_id]);
}

std::map<std::string, std::tuple<std::string, double>>& StatusLog::get_log() {
	return logbook;
}

void StatusLog::add_entry(const std::string& machine_id) {
	logbook[machine_id] = std::make_tuple("rebooting", time(&timer));
}

bool StatusLog::is_empty() {
	return logbook.empty();
}