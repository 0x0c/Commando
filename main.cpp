#include "Commando.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	if (argc == 4) {
		std::string ip_address = std::string(argv[1]);

		Commando comm(ip_address, atoi(argv[2]), atoi(argv[3]));
		comm.did_receive_handler = [](const boost::system::error_code& error, boost::array<char, 2048> received_data, std::size_t length) {
			if (error && error != asio::error::eof) {
				std::cout << error.message() << std::endl;
			}
			else {
				std::cout.write(received_data.data(), length);
			}
		};
		std::cout << "start" << std::endl;
		comm.receive();
	}
	else {
		std::cout << "argc < 4" << std::endl;
	}

	return 0;
}
