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
// class Response;
//class Request;
// class Webserver;
// class Server;

class	Socket
{
	public	:
		Socket (int optName, char *ip, uint16_t port);

        Socket();

        Socket (Socket const &obj);
        explicit Socket(int fdSock);

        ~Socket ();
		Socket &operator= (const Socket &obj);
        bool createServerSock(int optName,char* ip,uint16_t port,int type);
        bool setClientSock(int optName,char * ip,uint16_t port,int type);
    //choose what option socket hav to do: keepalive etc...

        //client side
        bool connectSocket(SOCKET clientSocket, uint16_t port);

    // int getFdSock() const;
    //
    // void setFdSock(int fdSock);
        int getFdSock() const;

        void setFdSock(int fd_sock);

        sockaddr_in &getService();

    void setService(const sockaddr_in &service);

    socklen_t *getSockSize();

    void setSockSize(socklen_t sockSize);

private	:
    //is necessary a pointer to server??
//        Server          *_server;
        int             _fd_sock;
        sockaddr_in     _service;
        socklen_t       _sockSize;
    bool _setSocketOption(int optName);
    bool _initializeService(char * ip,uint16_t port,int type);
    bool _bindSocket();
    bool _listenOnSocket();

    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
