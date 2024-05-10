#include "../include/Server.hpp"

Server::Server()
{
Socket servsock;
    this->_server_socket=servsock;
//    this->_server_socket.bindSocket();
//        socket.createSocket(server);
//        socket.setSocketOption(server);
//        socket.bindSocket(server);
//        socket.listenOnSocket(server.getServerSocket()->getFdSock());
//        fcntl(server.getServerSocket()->getFdSock(),F_SETFL,O_NONBLOCK);
	std::cout << "Server : Default Constructor Called" << std::endl;
}
Server::~Server()
{
	std::cout << "Server : Destructor Called" << std::endl;
}

Server::Server(Server const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Server	&Server::operator= (const Server &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
        this->_port=obj._port;
        this->_ip=obj._ip;
        this->_server_name=obj._server_name;
        this->_root=obj._root;
        this->_index=obj._index;
        this->_client_max_body_size=obj._client_max_body_size;
        this->_autoindex=obj._autoindex;
        this->_error_pages=obj._error_pages;
        this->_locations=obj._locations;
        this->_server_socket=obj._server_socket;
        this->_event=obj._event;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

/*
 * function
 */
/*
 * setup server:
 *  - call server constructor
 *  - create a socket per server, bind it and set it as non-blocking
 */
Server    setupServer()
{

    Socket sock;
//    //TODO maybe best practise is config subclass o server o inherit from it and we build server(conf) with copy constructor
    Server server;
//
//
//    server.setPort(conf.getPort());
//    server.setIp(conf.getIp());
//    server.setServerName(conf.getServerName());
//    server.setRoot(conf.getRoot());
//    server.setIndex(conf.getIndex());
//    server.setClientMaxBodySize(conf.getClientMaxBodySize());
//    server.setAutoindex(conf.isAutoindex());
//    server.setErrorPages(conf.getErrorPages());
//    server.setLocations(conf.getLocations());
////    server.setServerSocket(&sock);




    sock.createSocket(server);
    sock.setSocketOption(server);
    sock.bindSocket(server);
    sock.listenOnSocket(server.getServerSocket()->getFdSock());
    fcntl(server.getServerSocket()->getFdSock(),F_SETFL,O_NONBLOCK);
    return server;
    //build server objetc with parser info
    //    Server server(infobyparser);
}

/*
 * setup:
 */
//std::vector<Server>setup(std::vector<config> allConf)
//{
//
///*
// * create client and all is necessary to run the server
// * maybe both client and server go on the same list ???
// * if end without error go on on main
// */
//    std::vector<Server> listOfServer;
//    listOfServer.reserve(allConf.size());
//
//    for (std::vector<config>::iterator it = allConf.begin(); it != allConf.end(); ++it)
//    {
//
//        listOfServer.push_back(setupServer(*it));
//    }
//    return listOfServer;
//}


/*
 *
 *
 * getter and setter
 *
 *
 *
 *
 */

uint16_t Server::getPort() const {
    return _port;
}

void Server::setPort(uint16_t port) {
    _port = port;
}

const std::basic_string<char> Server::getIp() const {
    return _ip;
}

void Server::setIp(char *ip) {
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

const std::vector<std::string> Server::getErrorPages() const {
    return _error_pages;
}

void Server::setErrorPages(std::vector<std::basic_string<char> > errorPages) {
    _error_pages = errorPages;
}

const std::vector<Location> &Server::getLocations() const {
    return _locations;
}

void Server::setLocations(const std::vector<Location> &locations) {
    _locations = locations;
}

Socket *Server::getServerSocket() const {
    return _server_socket;
}

void Server::setServerSocket(Socket *serverSocket) {
    _server_socket = serverSocket;
}

const epoll_event &Server::getEvent() const {
    return _event;
}

void Server::setEvent(const epoll_event &event) {
    _event = event;
}
