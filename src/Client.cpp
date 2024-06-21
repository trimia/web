#include "../include/Client.hpp"

Client::Client() {
    this->_id = 0;
    this->_isLocation = false;
    this->_locationNumber = 0;
	this->_not_complete=false;
    this->_chunked=false;
    this->_cgi=false;
    this->_tempfilen=1;
//    std::cout << "Client : Default Constructor Called" << std::endl;
}


Client::~Client()
{
//	std::cout << "Client : Destructor Called" << std::endl;
    this->_id = 0;
    this->_isLocation = false;
    this->_locationNumber = 0;
    this->_not_complete=false;
    this->_chunked=false;
    this->_cgi=false;
    this->_tempfilen=1;
	delete this->_clientSock;
}

Client::Client(Client const &obj)
{
//	std::cout << "Client : Copy Constructor Called" << std::endl;
	if (this != &obj) {
		this->_id = obj._id;
		this->socketType = obj.socketType;
		this->_headerSize = obj._headerSize;
		this->_bodySize = obj._bodySize;
		this->_not_complete = obj._not_complete;
		this->_error = obj._error;
		this->_cgi = obj._cgi;
		this->_ended = obj._ended;
		this->_timeStart = obj._timeStart;
		this->_hasBeenClosed = obj._hasBeenClosed;
		this->_isLocation = obj._isLocation;
		this->_locationNumber = obj._locationNumber;
		this->_event = obj._event;
		this->_request = obj._request;
		this->_response = obj._response;
		this->set_locations(obj._locations);
		this->_clientSock = new Socket(*obj._clientSock);
	}

}

Client	&Client::operator= (const Client &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
        this->_id=obj._id;
		this->socketType = obj.socketType;
		this->set_client_sock(new Socket(*obj._clientSock));
		this->set_request(obj.request());
		this->_response=obj._response;
		this->_event=obj._event;
		this->_headerSize=obj._headerSize;
		this->_bodySize=obj._bodySize;
		this->_isLocation=obj._isLocation;
		this->_locationNumber=obj._locationNumber;
		this->set_locations(obj._locations);
	}
	return (*this);
}

bool Client::operator==(const Client &obj)const {
	return (this->_id == obj._id);
}

void Client::initClient(Server *server, int clientFd) {
	this->_id = clock();
	this->_root=server->getRoot();
    if(!server->getClientMaxBodySize().empty())
        this->_clientMaxBodySize=toInt(server->getClientMaxBodySize());
    else
        this->_clientMaxBodySize=0;
    _initSocket((char *) server->getIp().c_str(), server->getPort(), CLIENT_SOCK, clientFd);
    if (server->is_location()) {
    	this->_locations=server->getLocations();
        this->set_is_location(server->is_location());
        this->set_location_number(server->location_number());
    }
    this->_event.events = EPOLLIN;
    this->socketType = CLIENT_SOCK;
}

void Client::_initSocket(char *ip, uint16_t port, char type, int fd) {
	this->_clientSock=new Socket(fd);
	this->_clientSock->setClientSock(SO_RCVTIMEO,ip,port,type);
}

void Client::initRequest() {
	this->_request = new Request();
	if(this->_request->time_start()==0){
        this->_request->set_time_start(std::time(NULL));
        this->_timeStart=this->_request->time_start();
    }
}

void Client::initResponse() {
    this->_response = new Response();
	this->_response->set_root(this->_root);
}

Location Client::getTempLocation(std::vector<Location>& locations, std::string path_file) {
	Location bestMatch ;
	size_t bestMatchLenght = 0;
	if (!locations.empty())
		for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
			if (path_file.find(it->getPath()) == 0 && it->getPath().length() > bestMatchLenght) {
				bestMatch = *it;
				bestMatchLenght = it->getPath().length();
			}
		}
	return bestMatch;
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
	this->_clientSock->setFdSock(fd);
    return fd;
}

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

epoll_event & Client::event() {
	return _event;
}

void Client::set_event(epoll_event event) {
	_event = event;
}

std::string & Client::connection() {
	return _connection;
}

void Client::set_connection(std::string connection) {
	_connection = connection;
}

std::vector<Location>& Client::locations(){
	return _locations;
}

void Client::set_locations(const std::vector<Location> &locations) {
	std::vector<Location> newLocations;
	this->_locations.clear();
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		newLocations.push_back(*it);
	}
	this->_locations = newLocations;
}

bool & Client::get_not_complete() {
	return _not_complete;
}

void Client::set_not_complete(bool not_complete) {
	_not_complete = not_complete;
}

size_t Client::getClientMaxBodySize() const {
    return _clientMaxBodySize;
}

void Client::setClientMaxBodySize(size_t clientMaxBodySize) {
    _clientMaxBodySize = clientMaxBodySize;
}

int Client::getTempfilen() const {
    return _tempfilen;
}

void Client::setTempfilen(int tempfilen) {
    _tempfilen = tempfilen;
}

bool Client::isChunked() const {
    return _chunked;
}

void Client::setChunked(bool chunked) {
    _chunked = chunked;
}

