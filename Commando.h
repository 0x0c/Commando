#pragma once

#include <functional>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace asio = boost::asio;
using boost::asio::ip::udp;

class Commando
{
private:
	boost::asio::io_service send_io_service;
	udp::socket send_socket;
	udp::endpoint send_endpoint;

	boost::asio::io_service receive_io_service;
	udp::socket receive_socket;
	udp::endpoint receive_endpoint;
	boost::array<char, 2048> receive_buffer;

public:
	std::function<void(const boost::system::error_code error, boost::array<char, 2048> received_data, std::size_t length)> did_receive_handler;

	Commando(std::string ip_address, int send_port, int receive_port) : send_socket(send_io_service), receive_socket(receive_io_service, udp::endpoint(udp::v4(), receive_port)) {
		this->send_endpoint = udp::endpoint(asio::ip::address::from_string(ip_address), send_port);
		this->send_socket.open(udp::v4());
		this->send_io_service.run();
	};
	~Commando() {};
	
	void send(std::string str) {
		this->send_socket.async_send_to(boost::asio::buffer(str), this->send_endpoint, boost::bind(&Commando::send_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void send_handler(const boost::system::error_code& error, std::size_t length) {
		
	}

	void receive(bool async) {
		this->receive_socket.async_receive_from(boost::asio::buffer(this->receive_buffer), this->receive_endpoint, boost::bind(&Commando::receive_handler, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	}

	void receive_async() {
		boost::system::error_code error;
		size_t length = this->receive_socket.receive_from(boost::asio::buffer(this->receive_buffer), this->receive_endpoint, 0, error);
		if (this->did_receive_handler) {
			this->did_receive_handler(error, this->receive_buffer, length);
		}
	}

	void receive_handler(const boost::system::error_code error, std::size_t length) {
		if (this->did_receive_handler) {
			this->did_receive_handler(error, this->receive_buffer, length);
		}
		this->receive();
	}
};
