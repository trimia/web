#include "../include/Client.hpp"

Client::Client()
{
	std::cout << "Client : Default Constructor Called" << std::endl;
}

Client::Client(int id) : _id(id){}

Client::~Client()
{
	std::cout << "Client : Destructor Called" << std::endl;
}

Client::Client(Client const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Client	&Client::operator= (const Client &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
        this->_id=obj._id;
//        this->_server=obj;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

void Client::initSocket() {
	this->_clientSock = new Socket();

}

/*
 *
 *
 * getter and setter
 *
 */

Socket *Client::getClientSock() {
    return _clientSock;
}

int Client::setClientFdSock(int fd) {
	// this->_clientSock = new Socket();
	this->_clientSock->setFdSock(fd);
    return fd;
//    _clientSock.setService()
//    _clientSock = clientSock;
}
//
Request * Client::request() const {
	return _request;
}

void Client::set_request(Request *request) {
	_request = request;
}

Response * Client::response() const {
	return _response;
}

void Client::set_response(Response *response) {
	_response = response;
}

size_t Client::header_size() const {
	return _headerSize;
}

void Client::set_header_size(size_t header_size) {
	_headerSize = header_size;
}

size_t Client::body_size() const {
	return _bodySize;
}

void Client::set_body_size(size_t body_size) {
	_bodySize = body_size;
}
