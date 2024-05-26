#include "../include/Request.hpp"

Request::Request()
{
	std::cout << "Request : Default Constructor Called" << std::endl;
}

Request::~Request()
{
	this->_timeStart=0;
	this->_cgi=false;
	this->_error=false;
	this->_ended=false;
	this->_complete=false;
	std::cout << "Request : Destructor Called" << std::endl;
}

Request::Request(Request const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Request	&Request::operator= (const Request &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		this->_timeStart=obj._timeStart;
		this->_cgi=obj._cgi;
		this->_error=obj._error;
		this->_ended=obj._ended;
		this->_complete=obj._complete;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

/*
 *receiveData: old prototype must be upgrated read the fd and put it on buffer
 */

void Request::receiveData(Client *client)
{
	if(client->ended())
		return;
	std::cout<<BLUE<<"receiving data"<<std::endl;
	std::cout<<"client socket fd: "<<client->client_sock()->getFdSock()<<std::endl;
	std::cout<<"client socket service: "<<ntohs(client->client_sock()->getService().sin_port)<<std::endl;
	std::cout<<"client socket service: "<<inet_ntoa(client->client_sock()->getService().sin_addr)<<std::endl;
	std::cout<<"client socket size: "<<*client->client_sock()->getSockSize()<<std::endl;
	std::cout << "Service sin family: " << client->client_sock()->getService().sin_family<<RESET_COLOR<< std::endl;

	char rcv_buffer[RCV_BUF_SIZE];
	memset(rcv_buffer,0,RCV_BUF_SIZE);
	int byteCount=(int)recv(client->getClientSock()->getFdSock(),rcv_buffer, RCV_BUF_SIZE,0);
	if(byteCount==0)
	{

		std::cout<<"ready to close connection"<<std::endl;
		this->_complete=true;
	}else if(byteCount<0)
	{
		this->_error=true;
		std::cout<<"client read error"<<std::endl;
	}
	else if(byteCount !=0)
	{
		std::cout<<CYAN<<"buffer:"<<std::endl<<rcv_buffer<<RESET_COLOR<<std::endl;
		std::cout<<GREEN<<"receive data, "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		client->set_header_size(byteCount);
		client->set_ended(true);
		//understand if we want to register the parsed request in the client
		// client->request()->setRequestHeaders(client->request()->parseRequest(rcv_buffer));
		this->checktype(rcv_buffer);
		this->parseRequest(rcv_buffer);
		// return byteCount;
	}

}

 void Request::fillRequest() {

 	for(std::map<std::string, std::string>::iterator it = this->_request_headers.begin();it != this->_request_headers.end(); it++)
 	{
 		if(it->first=="")
 			this.=it->second;
 		if(it->first=="")
 			this.=it->second;
 		if(it->first=="")
 			this.=it->second;
 		if(it->first=="")
 			this.=it->second;
 	}
 }

 std::string Request::checktype(std::istream input) {
	std::string method;
	std::getline(input, method);
 	if (method.find("GET ") == 0 || method.find("POST ") == 0 || method.find("DELETE ") == 0 || method.find("HEAD ") == 0) {
 		return REQUEST;
 	} else { // Se non corrisponde a nessuno dei formati noti, messaggio HTTP non valido
 		return INVALID;
 	}
}

/*
 *parseRequest: parse http request all heather
 *necessary to fill information like hethere body method etc
 */
int Request::parseRequest(std::string input)
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
        	this->_request_headers[key]= value;
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




/*
 *getter & setter
 */
bool Request::error() const {
	return _error;
}

void Request::set_error(bool error) {
	_error = error;
}

bool Request::cgi() const {
	return _cgi;
}

void Request::set_cgi(bool cgi) {
	_cgi = cgi;
}

bool Request::ended() const {
	return _ended;
}

void Request::set_ended(bool ended) {
	_ended = ended;
}

bool Request::complete() const {
	return _complete;
}

void Request::set_complete(bool complete) {
	_complete = complete;
}

std::time_t Request::time_start() const {
	return _timeStart;
}

void Request::set_time_start(std::time_t time_start) {
	_timeStart = time_start;
}
void Request::setRequestHeaders(const std::map<std::string, std::string> &requestHeaders) {
	_request_headers = requestHeaders;
}

size_t Request::body_size() const {
	return _body_size;
}

void Request::set_body_size(size_t body_size) {
	_body_size = body_size;
}
