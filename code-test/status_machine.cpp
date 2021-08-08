#include "status_machine.h"

void StatusMachine::read_msg() {
	
	udp::endpoint sender_endpoint;
	time_t timer;
	double last_time = time(&timer);
	while (true) {

		size_t bytes = heartbeat_socket.available();
		if (bytes > 0)
		{
			size_t msg_len = heartbeat_socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
			parse_status(recv_buf, 2);			
		}

		bytes = startup_socket.available();
		if (bytes > 0)
		{
			size_t msg_len = startup_socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
			parse_status(recv_buf, 1);
		}

		poll_machines();
		
		
		if (time(&timer) - last_time > screen_update_interval)
		{
			print_status();
			last_time = time(&timer);
		}
	}
}

void StatusMachine::parse_status(const std::string& msg, int protocol) {
	std::vector<std::string> tokens;
	std::string token_builder = "";

	// split message string by | to get tokens
	for (char c : msg) {
		if (c != '|')
		{
			token_builder.push_back(c);
		}
		else
		{
			tokens.push_back(token_builder);
			token_builder.clear();
		}
	}
	// push back remainder tokens
	tokens.push_back(token_builder);

	// ignore other messages
	if (tokens.size() < 3)
		return;

	if (protocol == 2)
	{	
		std::string machine_id = tokens[1];
		std::string version = tokens[2];
		status_log.update_status(tokens[1], "active");
	}
	else
	{
		if (tokens[1] == "my session")
		{
			if (status_log.is_empty())
			{
				create_session(tokens);
			}
			std::string director_id = tokens[2];
			status_log.update_status(director_id, "rebooting");
		}
		else
		{
			std::string machine_id = tokens[1];
			status_log.update_status(machine_id, "rebooting");
		}
	}
}

void StatusMachine::create_session(std::vector<std::string> tokens) {
	for (unsigned int i = 2; i < tokens.size(); i++)
		status_log.add_entry(tokens[i]);
}

void StatusMachine::poll_machines() {

	std::map<std::string, std::tuple<std::string, double>> logbook = status_log.get_log();
	for(auto const& entry: logbook)
		// set machine status as inactive if not seen in last second
		if (status_log.last_seen(entry.first) > 1) 
			status_log.update_status(entry.first, "inactive");
}

void StatusMachine::print_status() {
	std::map<std::string, std::tuple<std::string, double>> logbook = status_log.get_log();
	std::string restarting = "Restarting: ";
	std::string inactive = "Inactive: ";
	std::string active = "Active: ";

	if (status_log.is_empty())
	{
		std::cout << "waiting for director to restart" << std::endl;
		return;
	}

	for (auto const& entry : logbook) {
		if (status_log.get_status(entry.first) == "inactive")
			inactive += entry.first + " |";

		else if (status_log.get_status(entry.first) == "active")
			active += entry.first + "  |";

		else
			restarting += entry.first + " |";
	}

	std::cout << active << "\n" << inactive << "\n" << restarting << "\n" << std::endl;
}