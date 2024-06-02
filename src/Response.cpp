#include "../include/Response.hpp"

#include <string>
#include <typeinfo>

#include "../include/Webserver.hpp"

extern Webserver* ws_ptr;


Response::Response()
{
	this->_header = "";
	this->_headerSize = 0;
	this->_body = "";
	this->_bodySize = 0;
	this->_content_type = "";
	this->_statusCode = 0;
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
	std::cout << "Response : Copy Constructor Called" << std::endl;
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
void Response::setResponseForMethod(Client *client) {
	std::cout<<BLUE<<"set response method"<<RESET_COLOR<<std::endl;
	// static int i = 1;
	// if (i == 0) {
	// 	i++;
	// 	return;
	// }
	// i--;
	// if(client->request()->method().empty())
	if(client->request()->error())
		return;
	// std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
	// // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	std::string method = client->request()->method();
	std::cout<<BLUE<<"method :"<<method<<RESET_COLOR<<std::endl;
	std::map<std::string, FunctionType> methodMap;
	methodMap["GET"] = &Response::getMethod;
	methodMap["POST"] = &Response::postMethod;
	methodMap["DELETE"] = &Response::deleteMethod;
	methodMap["PUT"] = &Response::putMethod;
	methodMap["HEAD"] = &Response::headMethod;
	(this->*methodMap[method])(client);

}


void Response::getMethod(Client *client)
{

	std::cout<<YELLOW<<"GETHMETHOD"<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
	// // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
	// std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	checkRequest(client);
	buildHttpResponseHeader(client->request()->http_version(),StatusString(this->status_code()),
							getMimeType(this->file_extension()),this->body_size());
	std::cout<<BLUE<<"GETHMETHOD"<<RESET_COLOR<<std::endl;
	return;
	// if(checkRequest(client)) {
	// 	std::cout<<BLUE<<"GETHMETHOD"<<RESET_COLOR<<std::endl;
	// 	return;
	// }

	// // buildHttpResponseHeader(client->request()->http_version(),client->response()->status_code(),
	// // 	StatusString(client->response()->status_code()),getMimeType(getFileExtension()),
	// // 	client->response()->header_size());
	//

	// //
	//
	// std::stringstream toStrin;
	// std::string msg="ZI PUO FAREEEEEEE!!!";
	// toStrin << msg.size();
	// this->_header = "HTTP/1.1 200 OK\n";
	// std::cout<<YELLOW<<"header: "<<this->_header<<RESET_COLOR<<std::endl;
	// this->_header += "Content-Type: text/plain\n";
	// this->_header += "Content-Length: " + toStrin.str() + "\n\n" +msg;
	// std::cout<<YELLOW<<"header: "<<this->_header<<RESET_COLOR<<std::endl;
	// this->_headerSize = this->_header.size();
	// std::cout<<YELLOW<<"header size: "<<this->_headerSize<<RESET_COLOR<<std::endl;
	// size_t responseSize = this->header_size() + msg.size();
	// char		*response = new char[responseSize];
	// memset(response, 0, responseSize);
	// for(size_t i=0; i < this->_headerSize;i++)
	// {
	// 	response[i]= this->_header.at(i);
	// }
	// std::cout<<YELLOW<<"response: "<<response<<RESET_COLOR<<std::endl;
	// int byteCount = (int)send(client->getClientSock()->getFdSock(),this->header().c_str(), this->header().length(), 0);
	// std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// if(byteCount==SOCKET_ERROR)
	// {
	// 	std::cout<<RED<<"send error"<<RESET_COLOR<<std::endl;
	// 	this->_error = true;
	// }else if (byteCount==0)
	// {
	// 	std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// }else if(byteCount !=0){
	// 	std::cout<<GREEN<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
	// 	this->_complete = true;
	// }
}
void Response::checkRequest(Client *client)
{
	std::cout<<BLUE<<"checkRequest"<<RESET_COLOR<<std::endl;
	// if (client->is_location())
	// {
	// 	this->_location=fitBestLocation(client);
	//
	// }
	if(!client->is_location())
	{
		std::cout<<RED<<"no location"<<RESET_COLOR<<std::endl;
		if(this->root().empty()|| this->root()=="/")
			this->_root="./www";
		else if(this->_root.find(".")==std::string::npos)
			this->_root="."+this->root();

		if(client->request()->is_rooth())
		{
			std::cout<<CYAN<<"root path"<<RESET_COLOR<<std::endl;
			this->_statusCode=200;
			this->_body="server is online";
			this->_bodySize=this->_body.length();
			this->_fileExtension=getFileExtension("txt");
			this->_readyToSend=true;
			return ;
		}
		if(client->request()->path_file().find_last_of(".") == std::string::npos) {
			std::cout<<CYAN<<"no extension"<<RESET_COLOR<<std::endl;
			isDirectory(this->root()+client->request()->path_file());
		}
		else
			readFromFile(this->root()+client->request()->path_file());
		std::cout<<YELLOW<<"path file: "<<this->root()+client->request()->path_file()<<RESET_COLOR<<std::endl;
		return ;
		// return sendData(client);
	}
	// TODO handle location
	// //check index autoindex
	// this->_path += "index.html";

	return ;
}

void Response::buildHttpResponseHeader(std::string httpVersion,
	std::string statusText, std::string contentType, size_t contentLength) {

	std::ostringstream header;
	header << httpVersion << " " << statusText << "\r\n";
	header << "Content-Type: " << contentType << "\r\n";
	header << "Content-Length: " << contentLength << "\r\n";
	header << "\r\n";  // End of header
	this->_headerSize = header.str().length();
	this->_readyToSend=true;
	this->_header= header.str();
}

void Response::sendData(Client *client)
{
	std::cout<<MAGENTA<<"send data complete: "<<this->complete()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"send data complete: "<<client->request()->complete()<<RESET_COLOR<<std::endl;
	if(this->complete())
		return;

	// buildHttpResponseHeader(client->request()->http_version(),StatusString(this->status_code()),
	//                         getMimeType(this->file_extension()),this->body_size());

	std::cout<<MAGENTA<<"body size :"<<this->body_size()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"body :"<<this->body()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"status code :"<<this->status_code()<<RESET_COLOR<<std::endl;
	// std::cout<<MAGENTA<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"status message"<<StatusString(this->status_code())<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"header size :"<<this->header_size()<<RESET_COLOR<<std::endl;
	std::cout<<MAGENTA<<"header :"<<this->header()<<RESET_COLOR<<std::endl;

	size_t responseSize = this->header().length() + this->body_size() + 4;
	char		*response = new char[responseSize];
	memset(response, 0, responseSize);
	for (size_t i=0; i < this->header().length(); i++)
		response[i]= this->header().at(i);
	for (size_t i=0; i < this->body_size(); i++)
		response[this->header_size()+i]= this->body().at(i);
	response[this->header_size() + this->body_size()] = '\r';
	response[this->header_size() + this->body_size() + 1] = '\n';
	response[this->header_size() + this->body_size() + 2] = '\r';
	response[this->header_size() + this->body_size() + 3] = '\n';
	std::cout<<YELLOW<<"response: "<<response<<" response size:"<<responseSize<<RESET_COLOR<<std::endl;
	//TODO https://linux.die.net/man/2/send see which falgs to use or if 0 is ok
	size_t byteCount = send(client->getClientSock()->getFdSock(),response, responseSize, MSG_DONTWAIT);
	//TODO understand if necessary
	// if(this->_statusCode==501)
	// 	return;
	//this status code is for internal server error log think if want to do it
	if((int)byteCount==SOCKET_ERROR)
	{
		std::cout<<RED<<"Error: not sended 500"<<RESET_COLOR<<std::endl;
		this->_statusCode = 500;
		if(epoll_ctl(ws_ptr->getEpollFd(),EPOLL_CTL_DEL,client->getClientSock()->getFdSock(),&client->event())==0)
			return;
		if(close(client->getClientSock()->getFdSock())!=-1)
			return;

		// this->_error = true;
	}
	else if (byteCount==0)
	{
		std::cout<<YELLOW<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		if(this->_statusCode==0)
			this->_statusCode = 204;
		// this->_complete = true;
		// return error handling
	}
	else if(byteCount==responseSize)
	{
		std::cout<<GREEN<<"send "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		this->_statusCode = 200;
		this->_complete = true;
	}
	// this->_complete = false;
}

void Response::readFromFile(std::string path) {
	std::cout<<CYAN<<"readFromFile"<<RESET_COLOR<<std::endl;
	// int	body_fd = open(path.c_str(), O_RDONLY);
	std::cout<<CYAN<<"path: "<<path<<RESET_COLOR<<std::endl;
	struct stat infoFile;
	stat(path.c_str(),&infoFile);
	if(infoFile.st_size<0) {
		std::cout<<RED<<"stat() error"<<RESET_COLOR<<std::endl;
	    this->_statusCode=404;
		return;
	}

	std::ifstream file(path.c_str(), std::ios::binary);
	// Check if the file was opened successfully
	if (!file.is_open()) {
		std::cout<<RED<<"file open error"<<RESET_COLOR<<std::endl;
		this->_statusCode=404;
		return;
	}
	// Read the file into a string
	std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	this->_body=body;
	std::cout<<CYAN<<"body: "<<this->_body<<RESET_COLOR<<std::endl;
	this->_bodySize=body.length();
	this->_fileExtension=getFileExtension(path);
	std::cout<<RED<<std::endl;
	printCharsAndSpecialChars(this->file_extension());
	std::cout<<RESET_COLOR<<std::endl;
	if(!this->_body.empty()) {
		this->_statusCode=200;// Close the file
		this->_readyToSend=true;
	}
	file.close();
}

void Response::isDirectory(const std::string& path) {
	std::cout<<CYAN<<"isDirectory"<<RESET_COLOR<<std::endl;
	struct stat info;
	if (stat(path.c_str(), &info) != 0) {
		std::cout<<RED<<"stat() error"<<RESET_COLOR<<std::endl;
		this->_statusCode=404;
		return;
	}
	if(path=="/") {
		std::cout<<CYAN<<"root path"<<RESET_COLOR<<std::endl;
		this->_statusCode=200;
		this->_body="server is online";
		this->_bodySize=this->_body.length();
		this->_readyToSend=true;

		return;
	}
	if(S_ISDIR(info.st_mode)){
		std::cout<<CYAN<<"is a directory"<<RESET_COLOR<<std::endl;
		this->_statusCode=200;
		this->_body="is a directory";
		this->_bodySize=this->_body.length();
		this->_readyToSend=true;
		return;
	}
}


// bool Response::checkLocation(Client *client) {
// 	// Trova la miglior corrispondenza della posizione per la richiesta
// 	client->response()->_location = checkIfExistLocation(client);
// 	// std::cout << CYAN_TEXT << "Best Location Match: " << client.request->bestLocation->loc_path
// 	// << "\nRoot: " << client.request->bestLocation->root << "\nIndex: " << client.request->bestLocation->index << RESET_COLOR << std::endl;
// 	// Verifica se il metodo é permesso
// 	if (allowMethod() == false)
// 		return false;
// 	// Ottieni il percorso completo considerando alias, root, e index
// 	getFullPath(client);
// 	return true;
// }
void Response::initLocation(Client *client) {
	fitBestLocation(client);
	if (!_location.allowMethod(client)) {
		this->_statusCode=405;
		return;
	}
	if(_location.autoindex()) {

	}

}

void Response::fitBestLocation(Client *client) {
	Location bestMatch;
	size_t bestMatchLenght = 0;
	// Itera attraverso le posizioni definite nel server
	for (std::vector<Location>::iterator it = client->locations().begin(); it != client->locations().end(); it++) {
		if (client->request()->path_file().find(it->getPath()) == 0 && it->getPath().length() > bestMatchLenght) {
			bestMatch = *it.base();
			bestMatchLenght = it->getPath().length();
		}
	}
	this->_location=bestMatch;
}
//
// bool Response::allowMethod(Client *client) {
// 	std::vector<std::string> methods = client->response()->location().getMethods();
// 	std::string method = client->request()->method();
// 	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
// 		if (it->compare(method) == 0)
// 			return true;
// 	}
// 	this->_statusCode = 405;
// 	return false;
// }

/*
 *
 *GETTER & SETTER
 *
 */

size_t Response::header_size() const {
	return _headerSize;
}

int Response::status_code() const {
	return _statusCode;
}

void Response::set_status_code(int status_code) {
	_statusCode = status_code;
}

Location Response::location() const {
	return _location;
}

void Response::set_location(const Location &location) {
	_location = location;
}

size_t Response::body_size() const {
	return _bodySize;
}

void Response::set_body_size(size_t body_size) {
	_bodySize = body_size;
}

std::string Response::body() const {
	return _body;
}

void Response::set_body(std::string body) {
	_body = body;
}

std::string Response::file_extension() {
	return _fileExtension;
}

void Response::set_file_extension(const std::string &file_extension) {
	_fileExtension = file_extension;
}

std::string Response::root() const {
	return _root;
}

void Response::set_root(std::string root) {
	_root = root;
}

bool Response::error() const {
	return _error;
}

bool Response::complete() const {
	return _complete;
}

void Response::set_complete(bool complete) {
	_complete = complete;
}

void Response::set_error(bool error) {
	_error = error;
}

bool Response::ready_to_send() const {
	return _readyToSend;
}

void Response::set_ready_to_send(bool ready_to_send) {
	_readyToSend = ready_to_send;
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
