#ifndef _Webserver_HPP_
#define _Webserver_HPP_

#include "include.h"
#include "Server.hpp"
#include "Client.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Client;
//class Server;

class	Webserver
{
	public	:
		Webserver ();
		Webserver (Webserver const &obj);
		~Webserver ();
		Webserver &operator= (const Webserver &obj);

    void addClientToList(Client client);

    const std::vector<Server> &getListOfServer() const;

    void setListOfServer(const std::vector<Server> &listOfServer);

    const std::vector<Client> &getListOfClient() const;

    void setListOfClient(const std::vector<Client> &listOfClient);

    int getEpollFd() const;

    void setEpollFd(int epollFd);

    void run_epoll();

private	:

    std::vector<Server>     _listOfServer;
    std::vector<Client>     _listOfClient;
    int                     _epollFd;
    bool _initEpoll();
    bool _addServerToEpoll();
    bool _mainLoop();
    bool _handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]);
    bool _handleConnection(epoll_event (&events)[MAX_EVENTS], int i);

		//	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
