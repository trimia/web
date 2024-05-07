#ifndef _Socket_HPP_
#define _Socket_HPP_

#include "include.h"
#include "Response.hpp"
#include "Request.hpp"
#include "Webserver.hpp"
#include "Server.hpp"


// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
//class Response;
//class Request;
class Webserver;
class Server;

class	Socket
{
	public	:
		Socket ();
		Socket (Socket const &obj);

    explicit Socket(int fdSock);

    ~Socket ();
		Socket &operator= (const Socket &obj);
        //choose what option socket hav to do: keepalive etc...
        bool createSocket(Server server);
        bool setSocketOption(Server server);
        //server side
        //maybe int is better because of bind listen and accept return an int 0 or errno
        //think about pass socket to function or the object server;
        bool bindSocket(Server server); //maybe the port is not necessary -> getport
        bool listenOnSocket(SOCKET serverSocket);
        bool acceptConnection(Server *server, int epollFd,Webserver *webserver);
        //client side
        bool connectSocket(SOCKET clientSocket, uint16_t port);
        int sendData(SOCKET connectedSocket, Response msg);
        int receiveData(SOCKET acceptedSocket,Request httpRequest);

    int getFdSock() const;

private	:
    //is necessary a pointer to server??
//        Server          *_server;
        int             _fd_sock;
        sockaddr_in     _service;
        socklen_t       _sockSize;

    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
