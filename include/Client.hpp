#ifndef _Client_HPP_
#define _Client_HPP_

//#include "Socket.hpp"
#include "include.h"
#include "Request.hpp"
#include "Response.hpp"
#include "Socket.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Socket;
class Server;
//class Response;
//class Request;
class	Client : public sType
{
	public	:
		Client ();
		Client (Client const &obj);

    Client(int id);

    ~Client ();
		Client &operator= (const Client &obj);

	private	:
        friend class Socket;



        int             _id;
//        int             _fd;
        Socket			*_clientSock;
        Server          *_server;
        Request		    *_request;
        Response        *_response;
        epoll_event     _event;
//        int             _type;


        int				_method;
        bool			_isFirstLine;
        bool			_isQuery;
        bool			_recvHeader;
        std::string		_cgiBody;
        size_t			_headerSize;
        size_t			_totSize;
        bool			_isContentLen;
        bool			_recvBlockSize;
        size_t			_readBlock;
        size_t			_blockSize;
        bool			_hasBeenClosed;
        //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
