#include "../include/Server.hpp"

Server::Server()
{
	std::cout << "Server : Default Constructor Called" << std::endl;
    this->_isCgiEnabled = false;
    this->_port = 0;
    this->_ip = "";
    this->_server_name = "";
    this->_root = "";
    this->_index = "";
    this->_client_max_body_size = "";
    this->_autoindex=false;
    this->_error_pages.reserve(0);
    this->_isLocation=false;
    this->_locationNumber = 0;
    this->_locations.reserve(0);
    this->_servSockCreatedWithNew = false;
    this->_server_socket=new Socket();
    // this->_allowmethods=std::vector<int>(METHOD_COUNT,0);
    // this->_servSockCreatedWithNew=false;
    // this->_locationNumber=0;
    // this->socketType=SERVER_SOCK;
}
Server::~Server()
{
    // if(serv_sock_created_with_new())
    delete this->_server_socket;
    if(this->_server_socket->getFdSock() != INVALID_SOCKET) {
        close(this->_server_socket->getFdSock());
    }
	std::cout << "Server : Destructor Called" << std::endl;
}

Server::Server(Server const &obj)
{
	std::cout << "Server : Copy Constructor Called" << std::endl;
	if (this != &obj) {
	    this->_server_name=obj._server_name;
	    this->socketType = obj.socketType;
	    this->_ip=obj._ip;
	    this->_port=obj._port;
	    this->_root=obj._root;
	    this->_server_socket = new Socket(*obj._server_socket);
        this->_isCgiEnabled = obj._isCgiEnabled;
	    this->_index=obj._index;
	    this->_client_max_body_size=obj._client_max_body_size;
	    this->_autoindex=obj._autoindex;
	    // this->_server_socket=obj._server_socket;
	    this->_event=obj._event;
	    this->setLocations(obj._locations);
	    this->setErrorPages(obj._error_pages);
	    this->_isLocation = obj._isLocation;
	    this->_locationNumber=obj._locationNumber;
	}
    *this = obj;
}

Server	&Server::operator= (const Server &obj)
{
    	std::cout << "Server : Copy Assignment Operator Called" << std::endl;
    if (this != &obj)
    {
        this->_server_name=obj._server_name;
        this->socketType = obj.socketType;
        this->_ip=obj._ip;
        this->_port=obj._port;
        this->_root=obj._root;
        this->_index=obj._index;
        this->_client_max_body_size=obj._client_max_body_size;
        this->_autoindex=obj._autoindex;
        this->_isCgiEnabled = obj._isCgiEnabled;
        // this->set_server_socket(new Socket(*obj._server_socket));

	    // this->_server_socket = new Socket(*obj._server_socket);
        // this->_server_socket=obj._server_socket;
        this->_event=obj._event;
        this->setLocations(obj._locations);
        this->setErrorPages(obj._error_pages);
        this->_isLocation = obj._isLocation;
        this->_locationNumber=obj._locationNumber;

        // this->_allowmethods=obj._allowmethods;
        // this->_isLocation=obj._isLocation;
        //	this->attributes = obj.attributes;
        //	...
    }
    return (*this);
}

/*
 * function
 */


/*
 * init sock:
 * SO_REUSADOR is a flag only for server!
 *  - create a socket per server, bind it and set it as non-blocking
 *      sock.createSocket(server);
 *      sock.setSocketOption(server);
 *      sock.bindSocket(server);
 *      sock.listenOnSocket(server.getServerSocket()->getFdSock());
 *      fcntl(server.getServerSocket()->getFdSock(),F_SETFL,O_NONBLOCK);
 */
void Server::initSock() {
    //TODO remember to free socket
    // this->_server_socket = new Socket(SO_REUSEADDR,(char *)this->_ip.c_str(),this->_port);
    // this->set_server_socket(new Socket());
    this->_server_socket=new Socket();
    this->_servSockCreatedWithNew=true;
    this->_server_socket->createServerSock(SO_REUSEADDR,(char *)this->_ip.c_str(),this->_port,SERVER_SOCK);
    // this->socketType=SERVER_SOCK;

}

/*
 *
 *
 * getter and setter
 *
 *
 *
 *
 */

bool    Server::getIsCgiEnabled() {
    return this->_isCgiEnabled;
}

void    Server::setIsCgiEnabled(bool cgiEnabled) {
    this->_isCgiEnabled = cgiEnabled;
}

uint16_t Server::getPort() const {
    return _port;
}

void Server::setPort(uint16_t port) {
    _port = port;
}

std::string &Server::getIp() {
    return _ip;
}

void Server::setIp(std::string ip) {
    _ip = ip;
}

const std::string &Server::getServerName() const {
    return _server_name;
}

void Server::setServerName(const std::string &serverName) {
    _server_name = serverName;
}

const std::string &Server::getRoot() const {
    return _root;
}

void Server::setRoot(const std::string &root) {
    _root = root;
}

const std::string &Server::getIndex() const {
    return _index;
}

void Server::setIndex(const std::string &index) {
    _index = index;
}

const std::basic_string<char> Server::getClientMaxBodySize() const {
    return _client_max_body_size;
}

void Server::setClientMaxBodySize(std::basic_string<char> clientMaxBodySize) {
    _client_max_body_size = clientMaxBodySize;
}

bool Server::isAutoindex() const {
    return _autoindex;
}

void Server::setAutoindex(bool autoindex) {
    _autoindex = autoindex;
}

std::vector<std::string> Server::getErrorPages() {
    return _error_pages;
}

void Server::setErrorPages(std::vector<std::string> errorPages) {
    for (std::vector<std::string>::iterator it = errorPages.begin();
         it != errorPages.end(); it++) {
        this->_error_pages.push_back(*it);
    }
}

std::vector<Location>& Server::getLocations() {
    return _locations;
}

void Server::setLocations(const std::vector<Location> &locations) {
    std::vector<Location> newLocations;

    for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        newLocations.push_back(*it);
    }
    this->_locations = newLocations;
}

Socket *Server::getserver_socket() {
    return _server_socket;
}

void Server::set_server_socket(Socket *server_socket) {
    _server_socket = server_socket;
}

bool Server::is_location(){
    return _isLocation;
}

void Server::setIsLocation(bool is_location) {
    _isLocation = is_location;
}

bool & Server::serv_sock_created_with_new() {
    return _servSockCreatedWithNew;
}

void Server::set_serv_sock_created_with_new(bool serv_sock_created_with_new) {
    _servSockCreatedWithNew = serv_sock_created_with_new;
}

int Server::location_number(){
    return _locationNumber;
}

void Server::set_location_number(int location_number) {
    _locationNumber = location_number;
}

// std::vector<int> Server::allowmethods() const {
//     return _allowmethods;
// }
//
// void Server::set_allowmethods(const std::vector<int> &allowmethods) {
//     _allowmethods = allowmethods;
// }

const epoll_event &Server::getEvent() const {
    return _event;
}

void Server::setEvent(const epoll_event &event) {
    _event = event;
}
