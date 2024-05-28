#ifndef _Response_HPP_
#define _Response_HPP_

#include "include.h"
#include "Client.hpp"
//#include "Request.hpp"
//#include "Server.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Client;
// class Request;
//class Server;

// ******************************************************** //
class	Response
{

public	:
	typedef void (Response::*FunctionType)(Client *client);
	Response ();
	Response (Response const &obj);
	~Response ();
	Response &operator= (const Response &obj);

	std::string buildHttpResponseHeader(std::string &httpVersion, int statusCode, std::string &statusText,
	                                    std::string &contentType, size_t contentLength);

	int sendData(Client *client, std::string body);
	void setResponseForMethod(Client *client);

	void getMethod(Client *client);

	void postMethod(Client *client);

	void deleteMethod(Client *client);

	void putMethod(Client *client);

	void headMethod(Client *client);

	std::string header() const;

	void set_header(const std::string &header);

	size_t header_size() const;

	void set_header_size(size_t header_size);

	const std::string &getContent() const;

private	:
	//we have to decide what we need here there were 2 example
	//first:
	//        Server          *_server;
	std::string     _header;
	size_t			_headerSize;
	std::string     _content_type;
	bool			_complete;
	bool			_error;
	std::string     _body;
	size_t			_bodySize;
	std::string     _response;
	size_t			_responseSize;
	int				_errorCode;


	std::string     _target_file;
	// std::vector<uint8_t> _body;
	size_t          _body_length;
	std::string     _response_body;
	std::string     _location;
	short           _code;
	char            *_res;
	int				_cgi;
	int				_cgi_fd[2];
	size_t			_cgi_response_length;
	bool            _auto_index;
	//second:
	// Request					*_request;
	bool					_isFinished;
	int						_statusCode;
	std::string				_resource;
	enum e_resource_type	_resourceType;
	std::string				_content;
	std::string				_contentType;
	std::string				_finalMsg;
	std::string				_index;
	//	DataType	attributes.
};

//                        FUNCTIONS                        //
// ****************************************************** //

#endif
