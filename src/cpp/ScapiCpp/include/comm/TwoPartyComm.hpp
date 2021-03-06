#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include "Message.hpp"
#include "Comm.hpp"

namespace boost_ip = boost::asio::ip; // reduce the typing a bit later...
using IpAdress = boost_ip::address;
using tcp = boost_ip::tcp;

/**
* A marker interface. Each type of party should have a concrete class that implement this interface.
*/
class PartyData{};

/**
* This class holds the data of a party in a communication layer.
* It should be used in case the user wants to use the regular mechanism of communication using tcp sockets.
*/
class SocketPartyData : public PartyData {
private:
	IpAdress ipAddress; // party's address.
	int port; // port number to listen on.
	int compare(const SocketPartyData &other) const;
public:
	SocketPartyData() {};
	/**
	* Constructor that sets the given arguments.
	* @param ip Party's address.
	* @param port Port number to listen on.
	*/
	SocketPartyData(IpAdress ip, int port) {
		ipAddress = ip;
		this->port = port;
	};
	IpAdress getIpAddress() { return ipAddress; };
	int getPort() { return port; };
	string to_log_string() {
		return ipAddress.to_string() + "|" + to_string(port);
	};
	/**
	* Compares two parties.
	*<0 if this party's string is smaller than the otherParty's string representation.
	*>0 if this party's string is larger than the otherParty's string representation.
	*/
	bool operator==(const SocketPartyData &other) const { return (compare(other) == 0); };
	bool operator!=(const SocketPartyData &other) const { return (compare(other) != 0); };
	bool operator<=(const SocketPartyData &other) const { return (compare(other) <= 0); };
	bool operator>=(const SocketPartyData &other) const { return (compare(other) >= 0); };
	bool operator>(const SocketPartyData &other) const { return (compare(other) > 0); };
	bool operator<(const SocketPartyData &other) const { return (compare(other) < 0); };
};



class NativeChannel {
public:
	NativeChannel(boost::asio::io_service& io_service_server, boost::asio::io_service& io_service_client,
		SocketPartyData me, SocketPartyData other) :
		io_service_client_(io_service_client), serverSocket(io_service_server), clientSocket(io_service_client)	{
		this->me = me;
		this->other = other;
		Logger::log("Native channel created. My IP: " + me.getIpAddress().to_string() + 
			" my port: " + to_string(me.getPort()) + " Peer's ip: " +
			other.getIpAddress().to_string() + " Peer's port: " + to_string(other.getPort()));
	};
	tcp::socket& getServerSocket() { return serverSocket; };
	void connect();
	void start_listening();
	void write(const Message& msg);
	void close();
	bool is_connected() { return m_IsConnected; };
	vector<byte> * NativeChannel::read_one();
	//vector<Message> read_all();

private:
	tcp::socket serverSocket;
	tcp::socket clientSocket;
	SocketPartyData me;
	SocketPartyData other;
	deque<Message> write_msgs_;
	Message read_msg_;
	deque<vector<byte> *> read_msgs_;
	boost::asio::io_service& io_service_client_;
	bool m_IsConnected = false;
	void handle_connect(const boost::system::error_code& error);
	void do_write(Message msg);
	void handle_write(const boost::system::error_code& error);
	void do_close() { clientSocket.close(); };
	void handle_msg(const Message& msg);
	void handle_read_body(const boost::system::error_code& error);
	void handle_read_header(const boost::system::error_code& error);
};

class ChannelServer {
private:
	tcp::acceptor acceptor_;
	boost::asio::io_service& io_service_server;
	boost::asio::io_service& io_service_client;
	NativeChannel * channel;
public:
	ChannelServer(boost::asio::io_service& io_service, SocketPartyData me, SocketPartyData other) :
		io_service_server(io_service), io_service_client(io_service),
		acceptor_(io_service, tcp::endpoint(tcp::v4(), me.getPort())) 
	{
		Logger::log("Craeting ChannelServer Beteen me (" + me.to_log_string() + ") and other (" + other.to_log_string() + ")");
		channel = new NativeChannel(io_service_server, io_service_client, me, other);
		acceptor_.async_accept(channel->getServerSocket(), boost::bind(&ChannelServer::handle_accept, 
			this, channel, boost::asio::placeholders::error));
	};
	void connect() { channel->connect(); };
	bool is_connected() { return channel->is_connected(); };
	void write(const Message& msg) { channel->write(msg); };
	vector<byte>* read_one() { return channel->read_one(); };
	//vector<Message> read_all() { return channel->read_all(); };

private:
	void handle_accept(NativeChannel * new_channel, const boost::system::error_code& error) {
		if (!error)
			new_channel->start_listening();
	};
};