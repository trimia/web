#include "../include/Response.hpp"

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

void Response::setResponseForMethod(Client *client) {

	switch (EXPRESSION)
	{
		case
	}

}

int Response::sendData(Client *client, std::string body)
{

	size_t responseSize = client->header_size() + client->body_size() + 4;
	Request requestPTR = reinterpret_cast<Request>(client->request());
	char		*response = new char[responseSize];
	memset(response, 0, responseSize);

	for(int i=0; i<client->header_size();i++)
	{
		response[i]= ;
	}
	// Response *ptr= reinterpret_cast<Response *>(client->response());

	int byteCount = (int)send(client->getClientSock().getFdSock(),response, sizeof(response), 0);
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
}



/*
 *
 *
 *GETTER & SETTER
 *
 *
 */
const std::string &Response::getContent() const {
	return _content;
}
