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
		this->set_request(obj.request());
		this->_response=obj._response;
		this->_event=obj._event;
		this->_headerSize=obj._headerSize;
		this->_bodySize=obj._bodySize;
		// this->_allowmethods=obj._allowmethods;
		this->_isLocation=obj._isLocation;
		this->_locationNumber=obj._locationNumber;
		this->set_locations(obj._locations);
		// this->_server=obj._server;
//        this->_server=obj;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

void Client::initClient(Server *server, int clientFd) {
	(void)clientFd;
	_initSocket((char *)server->getIp().c_str(),server->getPort(),CLIENT_SOCK,clientFd);
	set_is_location(server->is_location());
	set_location_number(server->location_number());
	set_locations(server->getLocations());
	this->_event.events=EPOLLIN | EPOLLOUT;
	this->socketType=CLIENT_SOCK;
}

void Client::_initSocket(char *ip, uint16_t port, char type, int fd) {
	// this->set_client_sock(new Socket());
	// std::cout<<RED<<"fd value   "<<fd<<RESET_COLOR<<std::endl;
	this->_clientSock=new Socket(fd);
	this->_clientSock->setClientSock(SO_RCVTIMEO,ip,port,type);

	// std::cout<<CYAN<<"init sock"<<std::endl;
	// std::cout<<"server socket fd: "<<this->_clientSock->getFdSock()<<std::endl;
	// std::cout<<"server socket service: "<<ntohs(this->_clientSock->getService().sin_port)<<std::endl;
	// std::cout<<"server socket service: "<<inet_ntoa(this->_clientSock->getService().sin_addr)<<std::endl;
	// std::cout<<"server socket size: "<<*this->_clientSock->getSockSize()<<std::endl;
	// std::cout << "Service sin family: " << this->_clientSock->getService().sin_family<<RESET_COLOR<< std::endl;

}

void Client::initRequest() {
	this->_request = new Request();
	// Request *request = new Request();
	// this->_request = request;

}

void Client::initResponse() {
	this->_response = new Response();
	// Response *response = new Response();
	// this->_response = response;
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

// std::vector<int> Client::allowmethods() const {
// 	return _allowmethods;
// }

// void Client::set_allowmethods(const std::vector<std::string> &allowmethods) {
// 	for (std::vector<std::string>::const_iterator it = allowmethods.begin();
// 		 it != allowmethods.end(); ++it) {
// 		if(it->c_str()=="GET")
// 			this->_allowmethods.push_back(GET);
// 		else if(it->c_str()=="POST")
// 			this->_allowmethods[POST]=1;
// 		else if(it->c_str()=="DELETE")
// 			this->_allowmethods[DELETE]=1;
// 		else if(it->c_str()=="PUT")
// 			this->_allowmethods[PUT]=1;
// 		else if(it->c_str()=="HEAD")
// 			this->_allowmethods[HEAD]=1;
// 		// else if(it->c_str()=="OPTIONS")
// 		// 	this->_allowmethods.push_back(OPTIONS);
// 		// else if(it->c_str()=="TRACE")
// 		// 	this->_allowmethods.push_back(TRACE);
// 		// else if(it->c_str()=="CONNECT")
// 		// 	this->_allowmethods.push_back(CONNECT);
// 		// else if(it->c_str()=="PATCH")
// 		// 	this->_allowmethods.push_back(PATCH);
// 		// else
// 		// 	this->_allowmethods.push_back(0);
//
// 	}
// }

bool Client::is_location() const {
	return _isLocation;
}

void Client::set_is_location(bool is_location) {
	_isLocation = is_location;
}

int Client::location_number() const {
	return _locationNumber;
}

void Client::set_location_number(int location_number) {
	_locationNumber = location_number;
}

std::vector<Location> Client::locations() const {
	return _locations;
}

void Client::set_locations(const std::vector<Location> &locations) {
	for (std::vector<Location>::const_iterator it = locations.begin();
		 it != locations.end(); ++it) {
		this->_locations.push_back(*it);
		 }
}
