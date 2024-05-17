#include "../include/Request.hpp"

Request::Request()
{
	std::cout << "Request : Default Constructor Called" << std::endl;
}

Request::~Request()
{
	std::cout << "Request : Destructor Called" << std::endl;
}

Request::Request(Request const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}
/*
 *receiveData: old prototype must be upgrated read the fd and put it on buffer
 */
int Request::receiveData(int acceptedSocket, Request httpRequest)
{
	char rcv_buffer[RCV_BUF_SIZE];
	memset(rcv_buffer,0,RCV_BUF_SIZE);
	int byteCount=(int)recv(acceptedSocket,rcv_buffer, RCV_BUF_SIZE,0);
	if(byteCount<=0)
	{
		std::cout<<"receive data error"<<GETSOCKETERRNO()<<std::endl;
		return SOCKET_ERROR;
	}else
	{
		std::cout<<"receive data, "<<byteCount<<" byte"<<std::endl;
		httpRequest.parseRequest(rcv_buffer,httpRequest);
		return byteCount;
	}

}

/*
 *parseRequest: parse http request all heather
 *necessary to fill information like hethere body method etc
 */


int Request::parseRequest(std::string input, Request httpRequest)
{
    size_t pos = 0;
    while ((pos = input.find("\n")) != std::string::npos)
    {
        std::string line = input.substr(0, pos);
        input.erase(0, pos + 1);
        if (line.empty())
			break;
        size_t colonPos = line.find_first_of(':');
        if (colonPos != std::string::npos)
        {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2); // Skip the colon and space
            httpRequest._request_headers[key] = value;
        }
    }
	// if(_request_headers["BODY"]=="")
		// ;
//fix this code with c++98
    // for (const auto& pair : httpRequest._request_headers) {
    //     std::cout << pair.first << ": " << pair.second << std::endl;
    // }
    return 0;
}

Request	&Request::operator= (const Request &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

/*
 *getter & setter
 */

void Request::setRequestHeaders(const std::map<std::string, std::string> &requestHeaders) {
	_request_headers = requestHeaders;
}
