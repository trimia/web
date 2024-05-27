#include "../include/Response.hpp"

#include <string>
#include <typeinfo>

Response::Response()
{
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

void Response::getMethod(Client *client)
{

	std::cout<<"getMethod"<<std::endl;
	// // std::stringstream itoa;
	// // itoa << this->header_size();
	// // this->_header = "HTTP/1.1 " + client._StatusString(client.getStatusCode()) + "\r\n";
	// this->_header = "HTTP/1.1 200 OK\r\n";
	// std::cout<<YELLOW<<"header: "<<this->_header<<RESET_COLOR<<std::endl;
	// // this->_header += "Content-Length: " + itoa.str() + "\r\n";
	// this->_header += "Content-Type: text/plain\n";
	// // client.response->header += "Content-Type: " + mimeTypes[getFileExtension(client.response->file_path)];
	// // if (client.request->fileName != "favicon.ico")
	// // 	client.response->header += "\r\nConnection: " + client.request->headers["Connection"];
	// // this->_header += "\r\n\r\n";
	// this->_headerSize = this->_header.size();
	// size_t responseSize = this->header_size() + client->request()->body_size() + 4;
	// char		*response = new char[responseSize];
	// memset(response, 0, responseSize);
	// // for(size_t i=0; i < this->_headerSize;i++)
	// // {
	// // 	response[i]= this->_header.at(i);
	// // }
	// std::cout<<YELLOW<<"response: "<<response<<RESET_COLOR<<std::endl;
	//
	// // for(size_t i=0; i < client->header_size();i++)
	// // {
	// // 	response[i]= client->request()->http_message().at(i);
	// // }
	// // Response *ptr= reinterpret_cast<Response *>(client->response());
	//
	//
	//
	// int byteCount = (int)send(client->getClientSock()->getFdSock(),this->header().c_str(), sizeof(this->header()), 0);
	// std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// if(byteCount==SOCKET_ERROR)
	// {
	// 	std::cout<<RED<<"send error"<<RESET_COLOR<<std::endl;
	// 	this->_error = true;
	// }else if (byteCount==0)
	// {
	// 	std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// 	// return error handling
	// }else if(byteCount !=0){
	// 	std::cout<<GREEN<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// 	this->_complete = true;
	// 	// return byteCount;
	// }
	// return 0;
}




void Response::setResponseForMethod(Client *client) {
	std::string method = client->request()->method();
	std::map<std::string, FunctionType> methodMap;
	methodMap["GET"] = &Response::getMethod;
	methodMap["POST"] = &Response::postMethod;
	methodMap["DELETE"] = &Response::deleteMethod;
	methodMap["PUT"] = &Response::putMethod;
	methodMap["HEAD"] = &Response::headMethod;
	(this->*methodMap[method])(client);


	// methodMap.insert(std::make_pair("GET", &Response::getMethod()));
	// methodMap.insert(std::make_pair(POST, postMethod()));
	// methodMap.insert(std::make_pair(DELETE, deleteMethod()));
	// methodMap.insert(std::make_pair(PUT, putMethod()));
	// methodMap.insert(std::make_pair(HEAD, headMethod()));

	// // std::map<std::string, HttpMethod> methodMap;
	// methodMap.insert(std::make_pair("POST", POST));
	// methodMap.insert(std::make_pair("DELETE", DELETE));
	// methodMap.insert(std::make_pair("PUT", PUT));
	// methodMap.insert(std::make_pair("HEAD", HEAD));

	// for (std::map<std::string, HttpMethod>::iterator it = methodMap.begin();
	// 		it != methodMap.end(); it++)
	// {
	// 	if (it->first == method)
	// 	{
	// 		this->_method = it->second;
	// 		break;
	// 	}
	// }
	// switch ()
	// {
	// 	case GET:
	// 		this->_method = GET;
	// 		break;
	// 	case PUT:
	// 		this->_method = PUT;
	// 		break;
	// 	case DELETE:
	// 		this->_method = DELETE;
	// 		break;
	// 	case POST:
	// 		this->_method = POST;
	// 		break;
	// 	case HEAD:
	// 		this->_method = HEAD;
	// 		break;
	// 	default:
	// 		this->_method = GET;
	// }
	// std::getline(input, method);
	// switch (EXPRESSION)
	// {
	// 	case
	// }

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
