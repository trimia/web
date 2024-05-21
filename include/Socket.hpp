#ifndef _Socket_HPP_
# define _Socket_HPP_

# include "include.h"
# include "Response.hpp"
# include "Request.hpp"
// # include "Webserver.hpp"
// # include "Server.hpp"



// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Response;
//class Request;
// class Webserver;
// class Server;

class	Socket
{
	public	:
		Socket ();
		Socket (Socket const &obj);
        explicit Socket(int fdSock);

        ~Socket ();
		Socket &operator= (const Socket &obj);
        bool createServerSock(int optName,char* ip,uint16_t port);
        bool setClientSock(int optName,char * ip,uint16_t port);
    //choose what option socket hav to do: keepalive etc...

        //client side
        bool connectSocket(SOCKET clientSocket, uint16_t port);
        int sendData(SOCKET connectedSocket, Response msg);

    int getFdSock() const;

    void setFdSock(int fdSock);

    const sockaddr_in &getService() const;

    void setService(const sockaddr_in &service);

    socklen_t getSockSize() const;

    void setSockSize(socklen_t sockSize);

private	:
    //is necessary a pointer to server??
//        Server          *_server;
        int             _fd_sock;
        sockaddr_in     _service;
        socklen_t       _sockSize;
    bool _setSocketOption(int optName);
    void _initializeService(char * ip,uint16_t port);
    bool _bindSocket(char * ip,uint16_t port); //maybe the port is not necessary -> getport
    bool _listenOnSocket();

    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
