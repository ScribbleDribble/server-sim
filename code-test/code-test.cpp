// code-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <asio.hpp>
#include "status_machine.h"
#define ASIO_STANDALONE 

int main() {
	try {
		asio::io_context io_context;
		StatusMachine status_machine(io_context, 7104, 7106);
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}

