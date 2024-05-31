#ifndef _Webserver_HPP_
#define _Webserver_HPP_

#include "include.h"
#include "Server.hpp"
#include "Client.hpp"
#include "ConfigParser.hpp"
#include "ConfigBlock.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Client;
//class Server;

class	Webserver
{
	public	:
		Webserver ();
        Webserver(char * conf);
		Webserver (Webserver const &obj);

    explicit Webserver(const std::string &conf);

    ~Webserver ();
		Webserver &operator= (const Webserver &obj);

    void addClientToList(Client *client);

    bool runEpoll();

	std::string readFromFile(std::string path);

    const std::vector<Server> &getListOfServer() const;

    void setListOfServer(const std::vector<Server> &listOfServer);

		std::list<Client *> getListOfClient() const;

    void setListOfClient(const std::list<Client *> listOfClient);

    int getEpollFd() const;

    void setEpollFd(int epollFd);

	// std::string _StatusString(int statusCode) {
	// 	switch (statusCode) {
	// 		case 100: return "100 Continue";
	// 		case 101: return "101 Switching Protocols";
	// 		case 200: return "200 OK";
	// 		case 201: return "201 Created";
	// 		case 204: return "204 No Content";
	// 		case 301: return "301 Moved Permanently";
	// 		case 302: return "302 Found";
	// 		case 304: return "304 Not Modified";
	// 		case 400: return "400 Bad Request";
	// 		case 401: return "401 Unauthorized";
	// 		case 403: return "403 Forbidden";
	// 		case 404: return "404 Not Found";
	// 		case 405: return "405 Method Not Allowed";
	// 		case 413: return "413 Request Entity Too Large";
	// 		case 500: return "500 Internal Server Error";
	// 		case 501: return "501 Not Implemented";
	// 		case 503: return "503 Service Unavailable";
	// 		case 504: return "504 Gateway Timeout";
	// 		case 505: return "505 HTTP Version Not Supported";
	// 		default: return "Unknown Status";
	// 	}
	// }

private	:

    std::vector<Server>     _listOfServer;
    std::list<Client*>		_listOfClient;
    int                     _epollFd;
    std::string             conf;
    bool _initEpoll();
    bool _addServerToEpoll();
    bool _mainLoop();
    bool _handleEpollEvents(int &eventNumber, epoll_event (&events)[MAX_EVENTS]);
    bool _handleConnection(epoll_event &event);


		bool _closeConnection(epoll_event &event);

		//	DataType	attributes.
    bool _acceptConnection(Server *server);
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
