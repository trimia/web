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
		// this->_clientSock=obj._clientSock;
		this->set_client_sock(obj._clientSock);
		this->_request=obj._request;
		this->_response=obj._response;
		this->_event=obj._event;
		this->_headerSize=obj._headerSize;
		this->_bodySize=obj._bodySize;
		// this->_server=obj._server;
//        this->_server=obj;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

void Client::initSocket(char *ip, uint16_t port, char type, int fd) {
	// this->set_client_sock(new Socket());
	std::cout<<RED<<"fd value   "<<fd<<RESET_COLOR<<std::endl;
	this->_clientSock=new Socket(fd);
	this->_clientSock->setClientSock(SO_RCVTIMEO,ip,port,type);
	std::cout<<CYAN<<"init sock"<<std::endl;
	std::cout<<"server socket fd: "<<this->_clientSock->getFdSock()<<std::endl;
	std::cout<<"server socket service: "<<ntohs(this->_clientSock->getService().sin_port)<<std::endl;
	std::cout<<"server socket service: "<<inet_ntoa(this->_clientSock->getService().sin_addr)<<std::endl;
	std::cout<<"server socket size: "<<*this->_clientSock->getSockSize()<<std::endl;
	std::cout << "Service sin family: " << this->_clientSock->getService().sin_family<<RESET_COLOR<< std::endl;

}

/*
 *
 * getter and setter
 *
 */

Socket *Client::getClientSock() {
    return _clientSock;
}

Socket * Client::client_sock() const {
	return _clientSock;
}

void Client::set_client_sock(Socket *client_sock) {
	_clientSock = client_sock;
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

bool Client::error() const {
	return _error;
}

void Client::set_error(bool error) {
	_error = error;
}

bool Client::cgi() const {
	return _cgi;
}

void Client::set_cgi(bool cgi) {
	_cgi = cgi;
}

bool Client::ended() const {
	return _ended;
}

void Client::set_ended(bool ended) {
	_ended = ended;
}

std::time_t Client::time_start() const {
	return _timeStart;
}

void Client::set_time_start(std::time_t time_start) {
	_timeStart = time_start;
}

bool Client::has_been_closed() const {
	return _hasBeenClosed;
}

void Client::set_has_been_closed(bool has_been_closed) {
	_hasBeenClosed = has_been_closed;
}
