#ifndef _Response_HPP_
#define _Response_HPP_

#include "include.h"
#include "Client.hpp"
#include "Location.hpp"
//#include "Request.hpp"
//#include "Server.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //
class Client;
// class Request;
//class Server;

class	Response
{

public	:
	typedef void (Response::*FunctionType)(Client *client);
	Response ();
	Response (Response const &obj);
	~Response ();
	Response &operator= (const Response &obj);


	void sendData(Client *client);

	void buildHttpResponseHeader(std::string httpVersion, std::string statusText,
										std::string &contentType, size_t contentLength);

	void readFromFile(std::string path);

	void isDirectory(const std::string &path);

	Location checkIfExistLocation(Client *client);

	bool allowMethod(Client *client);

	void setResponseForMethod(Client *client);

	void checkRequest(Client *client);

	void getMethod(Client *client);

	void postMethod(Client *client);

	void deleteMethod(Client *client);

	void putMethod(Client *client);

	void headMethod(Client *client);

	std::string header() const;

	void set_header(const std::string &header);

	size_t header_size() const;

	int status_code() const;

	void set_status_code(int status_code);

	Location location() const;

	void set_location(const Location &location);

	size_t body_size() const;

	void set_body_size(size_t body_size);

	std::string body() const;

	void set_body(std::string body);

	std::string file_extension();

	void set_file_extension(const std::string &file_extension);

	std::string root() const;

	void set_root(std::string root);

	bool error() const;

	bool complete() const;

	void set_complete(bool complete);

	void set_error(bool error);

	bool ready_to_send() const;

	void set_ready_to_send(bool ready_to_send);

	void set_header_size(size_t header_size);

	const std::string &getContent() const;

private	:
	//we have to decide what we need here there were 2 example
	//first:
	//        Server          *_server;
	std::string     _header;
	size_t			_headerSize;
	bool			_readyToSend;
	std::string     _content_type;
	bool			_complete;
	bool			_error;
	std::string     _body;
	size_t			_bodySize;
	std::string     _response;
	size_t			_responseSize;
	int				_statusCode;
	std::string     _path;
	std::string     _root;
	std::string     _fileExtension;


	std::string     _target_file;
	// std::vector<uint8_t> _body;
	size_t          _body_length;
	std::string     _response_body;
	Location     _location;
	short           _code;
	char            *_res;
	int				_cgi;
	int				_cgi_fd[2];
	size_t			_cgi_response_length;
	bool            _auto_index;
	//second:
	// Request					*_request;
	bool					_isFinished;
	std::string				_resource;
	enum e_resource_type	_resourceType;
	std::string				_content;
	std::string				_contentType;
	std::string				_finalMsg;
	std::string				_index;
	//	DataType	attributes.
};




// ******************************************************** //

//                        FUNCTIONS                        //
// ****************************************************** //

#endif
