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

    void addClientToList(Client client);

    bool runEpoll();

	std::string readFromFile(std::string path);

    const std::vector<Server> &getListOfServer() const;

    void setListOfServer(const std::vector<Server> &listOfServer);

    const std::vector<Client> &getListOfClient() const;

    void setListOfClient(const std::vector<Client> &listOfClient);

    int getEpollFd() const;

    void setEpollFd(int epollFd);

private	:

    std::vector<Server>     _listOfServer;
    std::vector<Client>     _listOfClient;
    int                     _epollFd;
    std::string             conf;
    bool _initEpoll();
    bool _addServerToEpoll();
    bool _mainLoop();
    bool _handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]);
    bool _handleConnection(epoll_event &event);
    bool _closeConnection(Client * client);

		//	DataType	attributes.
    bool _acceptConnection(Server *server);
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
