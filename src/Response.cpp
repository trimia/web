#include "../include/Response.hpp"

#include "../include/Webserver.hpp"

extern Webserver *ws_ptr;


Response::Response() {
    this->_header = "";
    this->_headerSize = 0;
    this->_body = "";
    this->_bodySize = 0;
    this->_content_type = "";
    this->_statusCode = 0;
    this->_complete = false;
    this->_error = false;
    this->_readyToSend = false;
    this->_location = new Location();

    std::cout << "Response : Default Constructor Called" << std::endl;
}

Response::~Response() {
    std::cout << "Response : Destructor Called" << std::endl;
    delete this->_location;
}


Response::Response(Response const &obj) {
    std::cout << "Response : Copy Constructor Called" << std::endl;
    if (this != &obj)
        *this = obj;
}

Response &Response::operator=(const Response &obj) {
    std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj) {
        this->_header = obj._header;
        this->_headerSize = obj._headerSize;
        this->_content_type = obj._content_type;
        this->_complete = obj._complete;
        this->_error = obj._error;
        //	this->attributes = obj.attributes;
        //	...
    }
    return (*this);
}

void Response::setResponseForMethod(Client *client) {
    std::cout << BLUE << "set response method" << RESET_COLOR << std::endl;
//	 static int i = 1;
//	 if (i == 0) {
//	 	i++;
//	 	return;
//	 }
//	 i--;
	 if(client->request()->method().empty())
         return;
    if (client->request()->error())
        return;
    // std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
    // // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
    std::string method = client->request()->method();
    std::cout << BLUE << "method :" << method << RESET_COLOR << std::endl;
    std::map<std::string, FunctionType> methodMap;
    methodMap["GET"] = &Response::getMethod;
    methodMap["POST"] = &Response::postMethod;
    methodMap["DELETE"] = &Response::deleteMethod;
    methodMap["PUT"] = &Response::putMethod;
    methodMap["HEAD"] = &Response::headMethod;
    (this->*methodMap[method])(client);

}


void Response::getMethod(Client *client) {

    std::cout << YELLOW << "GETHMETHOD" << RESET_COLOR << std::endl;
    // std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
    // // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
    checkRequest(client);
    buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
                            getMimeType(this->file_extension()), this->body_size());
    return;
}
void Response::postMethod(Client *client) {
    std::cout << YELLOW << "POSTMETHOD" << RESET_COLOR << std::endl;
    // std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
    // // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
    checkRequest(client);
    buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
                            getMimeType(this->file_extension()), this->body_size());
    return;
}


void Response::deleteMethod(Client *client) {
    (void) client;
    std::cout << "deleteMethod" << std::endl;
}

void Response::putMethod(Client *client) {
    (void) client;
    std::cout << "putMethod" << std::endl;
}

void Response::headMethod(Client *client) {
    (void) client;
    std::cout << "headMethod" << std::endl;
}


void Response::checkRequest(Client *client) {
    std::cout << BLUE << "checkRequest" << RESET_COLOR << std::endl;

    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "./www";
    if (this->_root.find(".") == std::string::npos)
        this->_root = "." + this->root();
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;
    if (location()->index().empty() && client->request()->path_file() == "/") {
        std::cout << CYAN << "root path" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = "server is online";
        this->_bodySize = this->_body.length();
        this->_fileExtension = getFileExtension("txt");
//        this->_readyToSend = true;
        return;
    }
    if (client->request()->path_file().find_last_of(".") == std::string::npos) {
        std::cout << CYAN << "no extension" << RESET_COLOR << std::endl;
        isDirectory(this->root() + client->request()->path_file());
    } else
        readFromFile(this->root() + client->request()->path_file());
    std::cout << YELLOW << "path file: " << this->root() + client->request()->path_file() << RESET_COLOR << std::endl;
    return;

}

void Response::handleLocation(Client *client) {

    std::cout << CYAN << "handleLocation" << RESET_COLOR << std::endl;
//    std::cout<<GREEN<<"location path: "<<this->_location->getPath()<<RESET_COLOR<<std::endl;
//    std::cout<<GREEN<<"location method: "<<this->_location->getMethods()[0]<<RESET_COLOR<<std::endl;
    std::cout<<YELLOW<<"fit best location"<<RESET_COLOR<<std::endl;
    std::cout << YELLOW<<"path file:" <<client->request()->path_file() << RESET_COLOR << std::endl;
    Location bestMatch;
    if (!client->locations().empty())
        for (std::vector<Location>::iterator it1 = client->locations().begin(); it1 != client->locations().end(); ++it1) {
            std::cout << BLUE << "LOC PATH : " << it1->getPath() << RESET_COLOR << std::endl;
            if (!it1->getMethods().empty()) {
                std::cout << BLUE << "LOC METHODS -> " << it1->getMethods()[0] << " : " << it1->getMethods()[1]
                          << RESET_COLOR << std::endl;
            }
        }
    else
        std::cout << RED << "LOCATIONS EMPTY" << RESET_COLOR << std::endl;
    size_t bestMatchLenght = 0;
    // Itera attraverso le posizioni definite nel server
    if (!client->locations().empty())
        for (std::vector<Location>::iterator it = client->locations().begin(); it != client->locations().end(); it++) {
            if (client->request()->path_file().find(it->getPath()) == 0 && it->getPath().length() > bestMatchLenght) {
                bestMatch = *it;
                bestMatchLenght = it->getPath().length();
                // std::cout << GREEN << "BEST MATCH PATH : " << bestMatch->getPath() << RESET_COLOR << std::endl;
                // std::cout << GREEN << "BEST MATCH METHOD -> " << bestMatch->getMethods()[0] << RESET_COLOR << std::endl;

            }
        }
    std::cout << GREEN << "BEST MATCH PATH : " << bestMatch.getPath() << RESET_COLOR << std::endl;
    std::cout << GREEN << "BEST MATCH METHOD -> " << bestMatch.getMethods()[0]<<" : "<< bestMatch.getMethods()[1] << RESET_COLOR << std::endl;


    if (!bestMatch.allowMethod(client->request()->method())) {
        std::cout << RED << "method not allowed" << RESET_COLOR << std::endl;
        this->_error = true;
        this->_statusCode = 405;
        return;
    }
    if (this->root().empty() || this->root() == "/" || bestMatch.root().empty())
        this->_root = "./www";
    else
        this->_root = bestMatch.root();
    if (this->_root.find(".") == std::string::npos)
        this->_root = "." + this->root();
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;

    std::cout << CYAN << "location autoindex: " << bestMatch.getAutoIndex() << RESET_COLOR << std::endl;
    if (bestMatch.getAutoIndex() && bestMatch.autoIndex(this->root() + client->request()->path_file())) {
        std::cout << CYAN << "autoindex" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = bestMatch.generateDirectoryListing(this->_root + client->request()->path_file());
        this->_bodySize = this->_body.length();
        this->_content_type = "html";
        this->_readyToSend = true;
        return;
    }
    if (!bestMatch.index().empty())
    {
        std::cout << CYAN << "index" << RESET_COLOR << std::endl;
        return readFromFile(this->location()->index());

    }
    if (bestMatch.alias().empty())
        if (int locationPathPos =
                client->request()->path_file().find_last_of(location()->getPath()) != std::string::npos)
            client->request()->path_file().replace(locationPathPos, location()->getPath().length(), location()->alias());
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(0) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(1) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(2) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(3) << RESET_COLOR << std::endl;
//        return readFromFile(this->location().root()+client->request()->path_file());
    //TODO client max body size set value for request and add a check in response building
//    return;

}

void Response::buildHttpResponseHeader(std::string httpVersion,
                                       std::string statusText, std::string contentType, size_t contentLength) {

    std::ostringstream header;
    header << httpVersion << " " << statusText << "\r\n";
    header << "Content-Type: " << contentType << "\r\n";
    header << "Content-Length: " << contentLength << "\r\n";
    header << "\r\n";  // End of header
    this->_headerSize = header.str().length();
//    this->_readyToSend = true;
    this->_header = header.str();
}

void Response::buildRedirectResponseHeader(std::string httpVersion,
                                           std::string statusText, std::string location) {

    std::ostringstream header;
    header << httpVersion << " " << statusText << "\r\n";
    header << "Location: " << location << "\r\n";
    header << "\r\n";  // End of header
    this->_headerSize = header.str().length();
//    this->_readyToSend = true;
    this->_header = header.str();
}

void Response::sendData(Client *client) {
    std::cout << MAGENTA << "send data complete: " << this->complete() << RESET_COLOR << std::endl;
    std::cout << MAGENTA << "send data complete: " << client->request()->complete() << RESET_COLOR << std::endl;
    if (this->complete())
        return;

    // buildHttpResponseHeader(client->request()->http_version(),StatusString(this->status_code()),
    //                         getMimeType(this->file_extension()),this->body_size());

//    std::cout << MAGENTA << "body size :" << this->body_size() << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "body :" << this->body() << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "time start :" << client->request()->time_start() << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "status code :" << this->status_code() << RESET_COLOR << std::endl;
//    // std::cout<<MAGENTA<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
//    std::cout << MAGENTA << "status message" << StatusString(this->status_code()) << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "method :" << client->request()->method() << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "header size :" << this->header_size() << RESET_COLOR << std::endl;
//    std::cout << MAGENTA << "header :" << this->header() << RESET_COLOR << std::endl;

    size_t responseSize = this->header().length() + this->body_size() + 4;
    char *response = new char[responseSize];
    memset(response, 0, responseSize);
    for (size_t i = 0; i < this->header().length(); i++)
        response[i] = this->header().at(i);
    for (size_t i = 0; i < this->body_size(); i++)
        response[this->header_size() + i] = this->body().at(i);
    response[this->header_size() + this->body_size()] = '\r';
    response[this->header_size() + this->body_size() + 1] = '\n';
    response[this->header_size() + this->body_size() + 2] = '\r';
    response[this->header_size() + this->body_size() + 3] = '\n';
    std::cout << YELLOW << "response: " << response << " response size:" << responseSize << RESET_COLOR << std::endl;
    //TODO https://linux.die.net/man/2/send see which falgs to use or if 0 is ok
    size_t byteCount = send(client->getClientSock()->getFdSock(), response, responseSize, MSG_DONTWAIT);
    //TODO understand if necessary
    // if(this->_statusCode==501)
    // 	return;
    //this status code is for internal server error log think if want to do it
    if ((int) byteCount == SOCKET_ERROR) {
        std::cout << RED << "Error: not sended 500" << RESET_COLOR << std::endl;
        this->_statusCode = 500;
        if (epoll_ctl(ws_ptr->getEpollFd(), EPOLL_CTL_DEL, client->getClientSock()->getFdSock(), &client->event()) == 0)
            return;
        if (close(client->getClientSock()->getFdSock()) != -1)
            return;

        // this->_error = true;
    } else if (byteCount == 0) {
        std::cout << YELLOW << "send " << byteCount << " byte" << RESET_COLOR << std::endl;
        if (this->_statusCode == 0)
            this->_statusCode = 204;
        // this->_complete = true;
        // return error handling
    } else if (byteCount == responseSize) {
        std::cout << GREEN << "send " << byteCount << " byte" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_complete = true;
    }
    // this->_complete = false;
}

void Response::readFromFile(std::string path) {
    std::cout << CYAN << "readFromFile" << RESET_COLOR << std::endl;
    // int	body_fd = open(path.c_str(), O_RDONLY);
    std::cout << CYAN << "path: " << path << RESET_COLOR << std::endl;
    struct stat infoFile;
    stat(path.c_str(), &infoFile);
    if (infoFile.st_size < 0) {
        std::cout << RED << "stat() error" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }

    std::ifstream file(path.c_str(), std::ios::binary);
    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cout << RED << "file open error" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }
    // Read the file into a string
    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    this->_body = body;
    std::cout << CYAN << "body: " << this->_body << RESET_COLOR << std::endl;
    this->_bodySize = body.length();
    this->_fileExtension = getFileExtension(path);
    std::cout << RED << std::endl;
    printCharsAndSpecialChars(this->file_extension());
    std::cout << RESET_COLOR << std::endl;
    if (!this->_body.empty()) {
        this->_statusCode = 200;// Close the file
//        this->_readyToSend = true;
    }
    file.close();
}

void Response::isDirectory(const std::string &path) {
    std::cout << CYAN << "isDirectory" << RESET_COLOR << std::endl;
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        std::cout << RED << "stat() error" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }
    if (path == "/") {
        std::cout << CYAN << "root path" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = "server is online";
        this->_bodySize = this->_body.length();
//        this->_readyToSend = true;

        return;
    }
    if (S_ISDIR(info.st_mode)) {
        std::cout << CYAN << "is a directory" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = "is a directory";
        this->_bodySize = this->_body.length();
//        this->_readyToSend = true;
        return;
    }
}

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

Location *Response::location() const {
    return _location;
}

void Response::set_location(Location *location) {
    std::cout << "Response : set_location" << std::endl;
    std::cout << "location path" << location->getPath() << std::endl;
    std::cout << "location method" << location->getMethods()[0]<<location->getMethods()[1] << std::endl;
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

std::string Response::header() const {
    return _header;
}

void Response::set_header(const std::string &header) {
    _header = header;
}

const std::string &Response::getContent() const {
    return _content;
}
