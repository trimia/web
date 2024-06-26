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


    // Server(uint16_t port, char *ip, std::string &serverName, std::string &root, std::string &index,
    //        unsigned long clientMaxBodySize, bool autoindex, std::vector<std::string> &errorPages,
    //        std::vector<Location> &locations, Socket *serverSocket);


    ~Server();
		Server  &operator= (const Server &obj);
		void initSock();
//        Server    setupServer(Co conf);
//        std::vector<Server> setup(std::vector<config> allConf);

/*
 *			getter & setter
 */

    // void unset() {
    //     this->_server_name = "";
    //     this->_ip = null;
    //     this->_port = 0;
    // };

//    bool getIsCgiEnabled(void);

    uint16_t getPort() const;

    void setPort(uint16_t port);

    std::string &getIp();

    void setIp(std::string ip);

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

    std::vector<std::string> getErrorPages();

    void setErrorPages(std::vector<std::string> errorPages);

    std::vector<Location>& getLocations();

    void setLocations(const std::vector<Location> &locations);

    const epoll_event &getEvent() const;

    void setEvent(const epoll_event &event);

	Socket *getserver_socket();

	void set_server_socket( Socket *server_socket);

    bool is_location();

    void setIsLocation(bool is_location);

    bool & serv_sock_created_with_new();

    void set_serv_sock_created_with_new(bool serv_sock_created_with_new);

    int location_number();

    void set_location_number(int location_number);

//    void setIsCgiEnabled(bool Enabled);



        // std::vector<int> allowmethods() const;
        //
        // void set_allowmethods(const std::vector<int> &allowmethods);

private	:
//        friend class Socket;
        friend class Webserver;
//        bool                            _isCgiEnabled;
        uint16_t						_port;
        std::string                     _ip;
        std::string						_server_name;
        std::string						_root;
        std::string						_index;
        std::basic_string<char>         _client_max_body_size;
        bool							_autoindex;
        std::vector<std::string>        _error_pages;
		bool							_isLocation;
		int								_locationNumber;
        std::vector<Location> 			_locations;
        Socket                          *_server_socket;
		bool							_servSockCreatedWithNew;
        epoll_event                     _event;
		// std::vector<int>				_allowmethods;
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
