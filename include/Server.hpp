#ifndef _Server_HPP_
#define _Server_HPP_

#include "include.h"
#include "Location.hpp"
#include "Socket.hpp"


//#include "Webserver.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
//class config;
class Socket;
//class Webserver;
class	Server : public sType
{
	public	:
        Server();
		Server(Server const &obj);


    Server(uint16_t port, char *ip, std::string &serverName, std::string &root, std::string &index,
           unsigned long clientMaxBodySize, bool autoindex, std::vector<std::string> &errorPages,
           std::vector<Location> &locations, Socket *serverSocket);

    ~Server();
		Server  &operator= (const Server &obj);
//        Server    setupServer(Co conf);
//        std::vector<Server> setup(std::vector<config> allConf);

    uint16_t getPort() const;

    void setPort(uint16_t port);

    const std::basic_string<char> getIp() const;

    void setIp(std::basic_string<char> ip);

    const std::string &getServerName() const;

    void setServerName(const std::string &serverName);

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    const std::string &getIndex() const;

    void setIndex(const std::string &index);

    const std::basic_string<char> getClientMaxBodySize() const;

    void setClientMaxBodySize(std::basic_string<char> clientMaxBodySize);

    bool isAutoindex() const;

    void setAutoindex(bool autoindex);

    const std::vector<std::string> getErrorPages() const;

    void setErrorPages(std::vector<std::basic_string<char> > errorPages);

    const std::vector<Location> &getLocations() const;

    void setLocations(const std::vector<Location> &locations);

    Socket *getServerSocket() const;

    void setServerSocket(Socket *serverSocket);

    const epoll_event &getEvent() const;

    void setEvent(const epoll_event &event);


private	:
        friend class Socket;
        friend class Webserver;
        uint16_t						_port;
        std::basic_string<char> _ip;
        std::string						_server_name;
        std::string						_root;
        std::string						_index;
        std::basic_string<char> _client_max_body_size;
        bool							_autoindex;
        std::vector<std::string>        _error_pages;
        std::vector<Location> 			_locations;
        Socket                          *_server_socket;
        epoll_event                     _event;
//        in_addr_t						_host;
//        struct sockaddr_in 				_server_address;
        //understand if _socket is necessary or _fd is good maybe have to change socket function return
//        int     						_fd;
//        int                             _epollFd;
//        int                             _type;
//        bool _initEpoll();
//        bool _addServerToEpoll(std::vector<Server> listofserver);
//        bool _mainLoop();
//        bool _handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]);
//        bool _handleConnection(epoll_event (&events)[MAX_EVENTS], int i);
		//	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
