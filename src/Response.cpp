#include "../include/Response.hpp"

#include <string>
#include <typeinfo>

Response::Response()
{
	this->_header = "";
	this->_headerSize = 0;
	this->_content_type = "";
	this->_complete = false;
	this->_error = false;

	std::cout << "Response : Default Constructor Called" << std::endl;
}

Response::~Response()
{
	std::cout << "Response : Destructor Called" << std::endl;
}


Response::Response(Response const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Response	&Response::operator= (const Response &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		this->_header=obj._header;
		this->_headerSize=obj._headerSize;
		this->_content_type=obj._content_type;
		this->_complete=obj._complete;
		this->_error=obj._error;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}
// void Response::setResponse(Client *client)
// {
// 	std::cout<<"setResponse"<<std::endl;
// 	std::string body = "Hello World!";
// 	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(body.length()) + "\n\n";
// 	std::string response = header + body;
// 	client->setResponse(response);
// }

std::string Response::buildHttpResponseHeader(std::string& httpVersion, int statusCode,
	std::string& statusText, std::string& contentType, size_t contentLength) {

	std::ostringstream header;
	header << httpVersion << " " << statusCode << " " << statusText << "\r\n";
	header << "Content-Type: " << contentType << "\r\n";
	header << "Content-Length: " << contentLength << "\r\n";
	header << "\r\n";  // End of header
	return header.str();
}

void Response::getMethod(Client *client)
{
	// std::cout<<BLUE<<"GETHMETHOD"<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
	// // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	//

	std::stringstream toStrin;
	// toStrin << client->request()->body_size();
	std::string msg="ZI PUO FAREEEEEEE!!!";
	toStrin << msg.size();
	// std::cout<<YELLOW<<"header size: "<<toStrin.str()<<RESET_COLOR<<std::endl;
	// this->_header = "HTTP/1.1 " + client._StatusString(client.getStatusCode()) + "\r\n";
	this->_header = "HTTP/1.1 200 OK\n";
	std::cout<<YELLOW<<"header: "<<this->_header<<RESET_COLOR<<std::endl;
	this->_header += "Content-Type: text/plain\n";
	this->_header += "Content-Length: " + toStrin.str() + "\n\n" +msg;
	// client.response->header += "Content-Type: " + mimeTypes[getFileExtension(client.response->file_path)];
	// if (client.request->fileName != "favicon.ico")
	// 	client.response->header += "\r\nConnection: " + client.request->headers["Connection"];
	// this->_header += "\r\n\r\n";
	std::cout<<YELLOW<<"header: "<<this->_header<<RESET_COLOR<<std::endl;
	this->_headerSize = this->_header.size();
	std::cout<<YELLOW<<"header size: "<<this->_headerSize<<RESET_COLOR<<std::endl;
	// size_t responseSize = this->header_size() + client->request()->body_size() + 4;
	size_t responseSize = this->header_size() + msg.size();
	char		*response = new char[responseSize];
	memset(response, 0, responseSize);
	for(size_t i=0; i < this->_headerSize;i++)
	{
		response[i]= this->_header.at(i);
	}
	std::cout<<YELLOW<<"response: "<<response<<RESET_COLOR<<std::endl;

	// for(size_t i=0; i < client->header_size();i++)
	// {
	// 	response[i]= client->request()->http_message().at(i);
	// }
	// Response *ptr= reinterpret_cast<Response *>(client->response());



	int byteCount = (int)send(client->getClientSock()->getFdSock(),this->header().c_str(), this->header().length(), 0);
	std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	if(byteCount==SOCKET_ERROR)
	{
		std::cout<<RED<<"send error"<<RESET_COLOR<<std::endl;
		this->_error = true;
	}else if (byteCount==0)
	{
		std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		// return error handling
	}else if(byteCount !=0){
		std::cout<<GREEN<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		this->_complete = true;
		// return byteCount;
	}
	// return 0;
}




void Response::setResponseForMethod(Client *client) {
	// std::cout<<BLUE<<"set response method"<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
	// // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	std::string method = client->request()->method();
	std::map<std::string, FunctionType> methodMap;
	methodMap["GET"] = &Response::getMethod;
	methodMap["POST"] = &Response::postMethod;
	methodMap["DELETE"] = &Response::deleteMethod;
	methodMap["PUT"] = &Response::putMethod;
	methodMap["HEAD"] = &Response::headMethod;
	(this->*methodMap[method])(client);

}
//
//
//
int Response::sendData(Client *client, std::string body)
{
	(void) body;
	size_t responseSize = client->request()->header_size() + client->request()->body_size() + 4;
	// Request *requestPTR = reinterpret_cast<Request*>(client->request());
	char		*response = new char[responseSize];
	memset(response, 0, responseSize);

	for(size_t i=0; i < client->header_size();i++)
	{
		response[i]= client->request()->http_message().at(i);
	}
	// Response *ptr= reinterpret_cast<Response *>(client->response());

	int byteCount = (int)send(client->getClientSock()->getFdSock(),response, sizeof(response), 0);
	if(byteCount==SOCKET_ERROR)
	{
		std::cout<<"send error"<<std::endl;
		return SOCKET_ERROR;
	}else if (byteCount==0)
	{
		std::cout<<"send "<<byteCount<<" byte"<<std::endl;
		// return error handling
	}else {
		std::cout<<"send "<<byteCount<<" byte"<<std::endl;
		return byteCount;
	}
	return 0;
}



/*
 *
 *
 *GETTER & SETTER
 *
 *
 */
size_t Response::header_size() const {
	return _headerSize;
}

void Response::set_header_size(size_t header_size) {
	_headerSize = header_size;
}

void Response::postMethod(Client *client)
{
	(void)client;
	std::cout<<"postMethod"<<std::endl;
}

void Response::deleteMethod(Client *client)
{
	(void)client;
	std::cout<<"deleteMethod"<<std::endl;
}

void Response::putMethod(Client *client)
{
	(void)client;
	std::cout<<"putMethod"<<std::endl;
}

void Response::headMethod(Client *client)
{
	(void)client;
	std::cout<<"headMethod"<<std::endl;
}

std::string Response::header() const {
	return _header;
}

void Response::set_header(const std::string &header) {
	_header = header;
}

const std::string &Response::getContent() const {
	return _content;
}
//
//
//
// // Server side C program to demonstrate HTTP Server programming
// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>
//
// #define PORT 8080
// int main(int argc, char const *argv[])
// {
// 	int server_fd, new_socket; long valread;
// 	struct sockaddr_in address;
// 	int addrlen = sizeof(address);
//
// 	// Only this line has been changed. Everything is same.
// 	char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
//
// 	// Creating socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("In socket");
// 		exit(EXIT_FAILURE);
// 	}
//
//
// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons( PORT );
//
// 	memset(address.sin_zero, '\0', sizeof address.sin_zero);
//
//
// 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
// 	{
// 		perror("In bind");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (listen(server_fd, 10) < 0)
// 	{
// 		perror("In listen");
// 		exit(EXIT_FAILURE);
// 	}
// 	while(1)
// 	{
// 		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
// 		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
// 		{
// 			perror("In accept");
// 			exit(EXIT_FAILURE);
// 		}
//
// 		char buffer[30000] = {0};
// 		valread = read( new_socket , buffer, 30000);
// 		printf("%s\n",buffer );
// 		write(new_socket , hello , strlen(hello));
// 		printf("------------------Hello message sent-------------------");
// 		close(new_socket);
// 	}
// 	return 0;
// }
// view rawHTTPServer.c hosted with ❤ by GitHub
