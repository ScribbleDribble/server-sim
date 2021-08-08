#pragma once
#include <asio.hpp>
#include <iostream>
#include "status_log.h"
#include <deque>

using asio::ip::udp;

class StatusMachine {

private:
	udp::endpoint endpoint1;
	udp::endpoint endpoint2;
	udp::socket startup_socket;
	udp::socket heartbeat_socket;
	asio::error_code ec;
	StatusLog status_log;
	bool is_setup = false;
	const static int max_len = 1024;
	char recv_buf[max_len];
	const double screen_update_interval = 2;

	void read_msg();
	void parse_status(const std::string& msg, int protocol);
	void poll_machines();
	void print_status();
	void create_session(std::vector<std::string> tokens);


public:

	StatusMachine(asio::io_context& io_context, int port1, int port2): endpoint1(asio::ip::address::from_string("127.0.0.1"), port1), 
		endpoint2(asio::ip::address::from_string("127.0.0.1"), port2),
		heartbeat_socket(io_context, endpoint1), startup_socket(io_context, endpoint2)
	{
		read_msg();
	}

};