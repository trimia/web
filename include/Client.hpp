#ifndef _Client_HPP_
#define _Client_HPP_

#include "include.h"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Location.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Server;
class Response;
class Request;
class Socket;
class Location;
class	Client : public sType
{
	public	:
		Client ();
		Client (Client const &obj);

    // Client(int id);

    ~Client ();
		Client &operator= (const Client &obj);

		bool operator==(const Client &obj) const;

		// bool &operator==(const Client &obj);
	void initClient(Server *server, int clientFd);
	void initRequest();
	void initResponse();
    void initLocation();

    Location *fitBestLocation(std::vector<Location>& locations, std::string path_file);

		Location getTempLocation(std::vector<Location> &locations, std::string path_file);


		// void initLocation();

		Socket *getClientSock();

		Socket * client_sock() const;

		void set_client_sock(Socket *client_sock);

		int setClientFdSock(int fd);

		Request * request() const;

		void set_request(Request *request);

		Response * response() const;

		void set_response(Response *response);

		size_t header_size() const;

		void set_header_size(size_t header_size);

		size_t body_size() const;

		void set_body_size(size_t body_size);

		bool error() const;

		void set_error(bool error);

		bool cgi() const;

		void set_cgi(bool cgi);

		bool ended() const;

		void set_ended(bool ended);

		std::time_t time_start() const;

		void set_time_start(std::time_t time_start);

		bool has_been_closed() const;

		void set_has_been_closed(bool has_been_closed);

		// std::vector<int> allowmethods() const;
		//
		// void set_allowmethods(const std::vector<std::string> &allowmethods);

		bool is_location() const;

		void set_is_location(bool is_location);

		int location_number() const;

		void set_location_number(int location_number);

		epoll_event & event();

		void set_event(epoll_event event);

		std::string & connection();

		void set_connection(std::string connection);

		std::vector<Location>& locations();

		void set_locations(const std::vector<Location> &locations);

		bool & get_not_complete();

		void set_not_complete(bool not_complete);

		size_t getClientMaxBodySize() const;

    void setClientMaxBodySize(size_t clientMaxBodySize);

    int getTempfilen() const;

    void setTempfilen(int tempfilen);

private	:
//        friend class Socket;
    friend class Webserver;




        int							_id;
        int                         _tempfilen;
        Socket						*_clientSock;
        Server						*_server;
        Request						*_request;
        Response					*_response;
        epoll_event					_event;
		// std::vector<int>			_allowmethods;
		bool						_isLocation;
		int							_locationNumber;
		std::vector<Location>		_locations;
		std::string					_connection;
        size_t                      _clientMaxBodySize;
		std::time_t					_timeStart;
		bool						_not_complete;
		void _initSocket(char*ip, uint16_t port, char type, int fd);


		bool            _error;
		bool            _cgi;
		bool            _ended;
		bool			_hasBeenClosed;

		size_t			_headerSize;
		size_t			_bodySize;
		int				_method;
		bool			_isFirstLine;
		bool			_isQuery;
		bool			_recvHeader;
		std::string		_cgiBody;
		size_t			_totSize;
		bool			_isContentLen;
		bool			_recvBlockSize;
		size_t			_readBlock;
		size_t			_blockSize;
		//	DataType	attributes.

};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
