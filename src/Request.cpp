#include "../include/Request.hpp"

Request::Request()
{
	this->_timeStart=0;
	this->_cgi=false;
	this->_error=false;
	this->_ended=false;
	this->_complete=false;
	this->_body_size=0;
	this->_headerSize=0;
	this->_method="";
	this->_httpMessage="";

	std::cout << "Request : Default Constructor Called" << std::endl;
}

Request::~Request()
{
	// this->_timeStart=0;
	// this->_cgi=false;
	// this->_error=false;
	// this->_ended=false;
	// this->_complete=false;
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
		this->_error=obj._error;
		this->_cgi=obj._cgi;
		this->_ended=obj._ended;
		this->_complete=obj._complete;
		this->_timeStart=obj._timeStart;
		this->_body_size=obj._body_size;
		this->_requestHeaders=obj._requestHeaders;
		this->_method=obj._method;
		this->_headerSize=obj._headerSize;
		this->_httpMessage=obj._httpMessage;
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
	// std::cout<<MAGENTA<<"receiving data"<<std::endl;
	// std::cout<<"client socket fd: "<<client->client_sock()->getFdSock()<<std::endl;
	// std::cout<<"client socket service: "<<ntohs(client->client_sock()->getService().sin_port)<<std::endl;
	// std::cout<<"client socket service: "<<inet_ntoa(client->client_sock()->getService().sin_addr)<<std::endl;
	// std::cout<<"client socket size: "<<*client->client_sock()->getSockSize()<<std::endl;
	// std::cout << "Service sin family: " << client->client_sock()->getService().sin_family<<RESET_COLOR<< std::endl;

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
		this->_headerSize=byteCount;
		this->_ended=true;
		this->_httpMessage=checktype(rcv_buffer);
		parseRequest();
		fillRequest(this->_httpMessage);
		return;
		//understand if we want to register the parsed request in the client
		// client->request()->setRequestHeaders(client->request()->parseRequest(rcv_buffer));
		// this->checktype(rcv_buffer);
		// return byteCount;
	}
	std::cout<<MAGENTA<<"receive data"<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
	// std::cout<<MAGENTA<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<std::endl<<"http message :"<<std::endl<<client->request()->_httpMessage<<RESET_COLOR<<std::endl<<std::endl;
	std::cout<<MAGENTA<<"request headers :"<<client->request()->_requestHeaders["Host"]<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"request headers :"<<client->request()->_requestHeaders["User-Agent"]<<RESET_COLOR<<std::endl;

}

 void Request::fillRequest(std::string &httpRequest) {
	size_t methodEndPos = httpRequest.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	std::istringstream input(httpRequest);
	std::string firsline;
	std::getline(input, firsline);
	setUrlPathQuery(firsline);
 	for(std::map<std::string, std::string>::iterator it = this->_requestHeaders.begin();it != this->_requestHeaders.end(); it++)
 	{
 		if(it->first=="Connection")
 			this->_keep_alive=it->second;
 		// if(it->first=="")
 		// 	this.=it->second;
 		// if(it->first=="")
 		// 	this.=it->second;
 		// if(it->first=="")
 		// 	this.=it->second;
 	}
 }

void Request::setUrlPathQuery(std::string &url) {
	size_t methodEndPos = url.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	size_t urlStartPos = methodEndPos + 1;
	size_t urlEndPos = url.find(" ", urlStartPos);
	if (urlEndPos == std::string::npos)
		this->_isRequestURL=false;
	this->_requestURL=url.substr(urlStartPos, urlEndPos - urlStartPos);
	this->_query= getQueryFromHttpRequest(url);
	size_t patEndPos = url.find("?");
	this->_path=url.substr(urlStartPos, patEndPos - urlStartPos);


}

std::string Request::getQueryFromHttpRequest(std::string& httpRequest) {
	size_t queryStartPos = httpRequest.find("?");
	if (queryStartPos == std::string::npos) {
		this->_isQuery=false;
		return "";
	}
	size_t queryEndPos = httpRequest.find(" ", queryStartPos);
	if (queryEndPos == std::string::npos) {
		// The query is at the end of the HTTP request
		return httpRequest.substr(queryStartPos + 1);
	}
	return httpRequest.substr(queryStartPos + 1, queryEndPos - queryStartPos - 1);
}

 std::string Request::checktype(std::string httpRequest) {
	size_t methodEndPos = httpRequest.find(" ");
	if (methodEndPos == std::string::npos) {
		this->_error= true;
		// Handle error: Invalid HTTP request
		return "";
	}
	std::istringstream input(httpRequest);
	std::string method;
	input>> method;
	std::cout<<YELLOW<<"checktype method :"<<method<<RESET_COLOR<<std::endl;
	if (method.find("GET") == 0 || method.find("POST") == 0 || method.find("DELETE") == 0 || method.find("HEAD") == 0) {
		set_method(method);
	} else { // Se non corrisponde a nessuno dei formati noti, messaggio HTTP non valido
		this->_error = true;
	}
	int lastLineStart=httpRequest.find("Content-Length: ");
	int numberEnd=httpRequest.find("\r\n\r\n",lastLineStart);
	std::cout<<RED<<"numberEnd :"<<numberEnd<<RESET_COLOR<<std::endl;
	this->_body_size=toInt((char *)httpRequest.substr(lastLineStart+16,numberEnd-lastLineStart-16).c_str());

	std::cout<<YELLOW<<"checktype body size :"<<this->_body_size<<RESET_COLOR<<std::endl;

	//maybe we can delete this:
	size_t pos = httpRequest.find("\n");
	if (pos != std::string::npos) {
		// Erase the first line including the newline character
		httpRequest.erase(0, pos + 1);
	}
	return httpRequest;
}

/*
 *parseRequest: parse http request all heather
 *necessary to fill information like hethere body method etc
 */

int Request::parseRequest()
{
	std::cout<<YELLOW<<"parseRequest"<<RESET_COLOR<<std::endl;
	std::string input= this->_httpMessage;
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
        	this->_requestHeaders[key]= value;
        }
    }

	// print for debug
     // for (const auto& pair : this->_requestHeaders) {
     //     std::cout<<CYAN << pair.first << " first : second " << pair.second << RESET_COLOR<<std::endl;
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

size_t Request::body_size() const {
	return _body_size;
}

void Request::set_body_size(size_t body_size) {
	_body_size = body_size;
}

std::string Request::method() const {
	return _method;
}

void Request::set_method(const std::string &method) {
	_method = method;
}

size_t Request::header_size(){
	return _headerSize;
}

void Request::set_header_size(size_t header_size) {
	_headerSize = header_size;
}
inline std::map<std::string, std::string> Request::request_headers() const {
	return _requestHeaders;
}

inline void Request::set_request_headers(const std::map<std::string, std::string> &request_headers) {
	_requestHeaders = request_headers;
}

std::string Request::http_message(){
	return _httpMessage;
}

inline void Request::set_http_message(const std::string &http_message) {
	_httpMessage = http_message;
}