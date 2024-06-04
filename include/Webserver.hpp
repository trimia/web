#ifndef _Webserver_HPP_
#define _Webserver_HPP_

#include "include.h"
#include "Server.hpp"
#include "Client.hpp"
#include "Cgi.hpp"
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

    const std::map<int, size_t> &getClientMaxBodySize() const;

    void setClientMaxBodySize(const std::map<int, size_t> &clientMaxBodySize);

    void setEpollFd(int epollFd);

private	:
    std::vector<Server>             _listOfServer;
    std::list<Client*>		             _listOfClient;
    int                                         _epollFd;
    std::string                               conf;
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
