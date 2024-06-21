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
    this->return_= false;

//    std::cout << "Response : Default Constructor Called" << std::endl;
}

Response::~Response() {
//    std::cout << "Response : Destructor Called" << std::endl;
    this->_header = "";
    this->_headerSize = 0;
    this->_body = "";
    this->_bodySize = 0;
    this->_content_type = "";
    this->_statusCode = 0;
    this->_complete = false;
    this->_error = false;
    this->_readyToSend = false;
    this->return_= false;
}


Response::Response(Response const &obj) {
//    std::cout << "Response : Copy Constructor Called" << std::endl;
    if (this != &obj)
        *this = obj;
}

Response &Response::operator=(const Response &obj) {
//    std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj) {
        this->_header = obj._header;
        this->_headerSize = obj._headerSize;
        this->_content_type = obj._content_type;
        this->_complete = obj._complete;
        this->_error = obj._error;
    }
    return (*this);
}

void Response::setResponseForMethod(Client *client) {
    std::cout << BLUE << "set response method" << RESET_COLOR << std::endl;
    if(client->request()->method().empty())
        return;
    if (client->request()->error())
        return;
    std::string method = client->request()->method();
    std::map<std::string, FunctionType> methodMap;
    methodMap["GET"] = &Response::getMethod;
    methodMap["POST"] = &Response::postMethod;
    methodMap["DELETE"] = &Response::deleteMethod;
    (this->*methodMap[method])(client);
    buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
                            getMimeType(this->file_extension()), this->body_size());

}


void Response::getMethod(Client *client) {
    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (!this->_root.empty() && this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    if (!client->is_location() && client->request()->path_file() == "/") {
        std::cout << CYAN << "root path" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = "server is online";
        this->_bodySize = this->_body.length();
        this->_fileExtension = getFileExtension("txt");
        return;
    }
    if (client->request()->path_file().find_last_of(".") == std::string::npos) {
        isDirectory(this->root() + client->request()->path_file());
    } else if (!client->cgi())
        readFromFile(this->root() + client->request()->path_file(),client);
    else
    return;
}

void Response::postMethod(Client *client) {
    if(client->get_not_complete())
        return;
    std::string str;

    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (!this->_root.empty() && this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    std::string path;
    struct stat infoFile;
    if(client->request()->path_file() == "/") {
        path=this->_root+"/upload";
    }
    else {
        path=this->_root+client->request()->path_file();
    }
    stat((path).c_str(), &infoFile);

    if (S_ISDIR(infoFile.st_mode)) {
        if(client->request()->file_name().empty())
            client->request()->set_file_name("temp");
        if(client->request()->getExtension().empty())
            client->request()->set_extension(getExtensionFromMimeType(client->request()->getMyme_type()));
        struct  stat info;
        stat((path+"/"+client->request()->file_name()+"."+client->request()->getExtension()).c_str(),&info);
        if(S_ISREG(info.st_mode))
        {

            std::string n= toStr(client->getTempfilen());
            if(client->getTempfilen()==0)
                n="";
            str=path+"/"+client->request()->file_name()+"("+n+")"+"."+client->request()->getExtension();
            client->setTempfilen(client->getTempfilen()+1);
        }else
            str=path+"/"+client->request()->file_name()+"."+client->request()->getExtension();
        std::ofstream file(str.c_str(), std::ios::app | std::ios::binary);
        file.write(client->request()->getBody().c_str(), client->request()->getBody().size());
        if(file.fail())
        {
            std::cout << RED << "Error in file write" << RESET_COLOR << std::endl;
            this->_statusCode = 500;
            return;
        }
        file.close();
        this->_statusCode = 201;
    }else {
        std::cout << RED << "Error in stat()" << RESET_COLOR << std::endl;
        this->_statusCode = 409;
        return;
    }
}


void Response::deleteMethod(Client *client) {

    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (!this->_root.empty() && this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    std::string uri=client->request()->path_file();
    std::string fileName=uri.substr(client->request()->path_file().find_last_of("/"));
    std::string path=uri.substr(0,client->request()->path_file().find_last_of("/"));
    std::string dirPath = this->_root + path;
    if(access(dirPath.c_str(), W_OK) == 0) {
        if (std::remove((dirPath+"/"+fileName).c_str()) == 0) {
            this->_statusCode = 204;
            return;
        } else {
            this->_statusCode = 500;
            return;
        }
    } else {
        this->_statusCode = 403;
        return;
    }
}

void Response::checkPath(Client *client) {
    size_t rootPos=client->request()->path_file().find(this->_root);
    if(rootPos != std::string::npos){
        std::string temp;
        temp=client->request()->path_file().erase(rootPos, this->_root.length());
        client->request()->set_path_file(temp);
    }
    if(client->request()->path_file().find_last_of("/")==client->request()->path_file().length()){
        std::string temp;
        temp=client->request()->path_file().substr(0,client->request()->path_file().length()-1);
        client->request()->set_path_file(temp);
    }
}

void Response::handleLocation(Client *client) {
    Location bestMatch;
    size_t bestMatchLenght = 0;
    if (!client->locations().empty())
        for (std::vector<Location>::iterator it = client->locations().begin(); it != client->locations().end(); ++it) {
            if (client->request()->path_file().find(it->getPath()) != std::string::npos && it->getPath().length() > bestMatchLenght) {
                bestMatch = *it;
                bestMatchLenght = it->getPath().length();
            }
        }
    if (!bestMatch.getMethods().empty() && !bestMatch.allowMethod(client->request()->method())) {
        this->_error = true;
        this->_statusCode = 405;
        return;
    }
    if (bestMatch.isIsRoot() &&(this->root() == "/" || bestMatch.root().empty()))
        this->_root = "/www";
    else if(bestMatch.isIsRoot())
        this->_root = bestMatch.root();
    checkPath(client);
    if (!this->_root.empty() && this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    if (!bestMatch.alias().empty()){
        size_t locationPathPos = client->request()->path_file().find(bestMatch.getPath());
        if ( locationPathPos  != std::string::npos){
            std::string temp;
            temp=client->request()->path_file().replace(locationPathPos, bestMatch.getPath().length(), bestMatch.alias());
            client->request()->set_path_file(temp);
        }
    }
    if (bestMatch.getAutoIndex() && bestMatch.autoIndex(this->root() + client->request()->path_file())) {
        if (bestMatch.getIsCgi())
            return;
        this->_statusCode = 200;
        this->_body = bestMatch.generateDirectoryListing( client->request()->path_file(),this->root());
        this->_bodySize = this->_body.length();
        this->_content_type = "html";
        this->_readyToSend = true;
        return;
    }
    if (!bestMatch.index().empty() && !bestMatch.getIsCgi())
    {
        return readFromFile(bestMatch.index(),client);
    }
    if (!bestMatch.getReturn().empty() && !bestMatch.getIsCgi()) {
        std::vector<std::string> return_ = bestMatch.getReturn();
        for (std::vector<std::string>::iterator it = return_.begin(); it != return_.end(); ++it) {
            if(isDigits(*it)){
                this->_statusCode=toInt(*it);
                this->_error=true;
            }
            else if(it->compare("return")!=0)
                this->_location=*it;
        }
        this->return_= true;
        this->_readyToSend = true;
        return;
    }
    if(bestMatch.getIsCgi())
    {
        Cgi cgi(client->request());
        cgi.executeCgi(client);
        client->set_cgi(true);
    }
}

void Response::buildHttpResponseHeader(std::string httpVersion,
                                       std::string statusText, std::string contentType, size_t contentLength) {
    std::ostringstream header;
    header << httpVersion << " " << statusText << "\r\n";
    header << "Content-Type: " << contentType << "\r\n";
    header << "Content-Length: " << contentLength << "\r\n";
    header << "\r\n";  // End of header
    this->_headerSize = header.str().length();
    this->_header = header.str();
}

void Response::buildRedirectResponseHeader(std::string httpVersion,
                                           std::string statusText, std::string location) {
    std::ostringstream header;
    header << httpVersion << " " << statusText << "\r\n";
    header << "Location: " << location << "\r\n";
    header << "\r\n";  // End of header
    this->_headerSize = header.str().length();
    this->_header = header.str();
}

static size_t send_all(int socket, const char* buffer, size_t length, int flags) {
    size_t total_bytes_sent = 0;
    while (total_bytes_sent < length) {
        size_t bytes_sent = send(socket, buffer + total_bytes_sent, length - total_bytes_sent, flags);
        if ((int)bytes_sent == -1) {
            return -1;
        }
        total_bytes_sent += bytes_sent;
    }
    return total_bytes_sent;
}

void Response::sendData(Client *client) {
    if (this->complete())
        return;

    if(this->status_code()>=400 && this->status_code()<=505)
    {
        client->response()->readFromFile(getErrorPages(client->response()->status_code()),client);
        client->response()->buildHttpResponseHeader(client->request()->http_version(), StatusString(client->response()->status_code()),
                                                    getMimeType(client->response()->file_extension()), client->response()->body_size());
    }
    std::ostringstream response;
    size_t responseSize = 0;
    if(!client->isChunked()) {
        std::cout << YELLOW << "not chunked" << RESET_COLOR << std::endl;
        response.clear();
        response << this->header();
        response << this->body();
        response << "\r\n\r\n";
        responseSize = response.str().length();
    } else
    {
        std::cout << YELLOW << "chunked" << RESET_COLOR << std::endl;
        response.clear();
        response << client->request()->http_version() << " " << StatusString(client->response()->_statusCode) << "\r\n";
        response << "Content-Type: " << getMimeType(client->response()->_fileExtension) << "\r\n";
        response << "Transfer-Encoding: chunked\r\n";
        response << "\r\n";
        size_t chunkSize = 4096;  // Size of each chunk
        for (size_t i = 0; i < this->_body.length(); i += chunkSize) {
            size_t currentChunkSize = std::min(chunkSize, this->_body.length() - i);
            response << std::hex << currentChunkSize << "\r\n";
            response << this->_body.substr(i, currentChunkSize);
            response << "\r\n";
        }
        response << "0\r\n\r\n";
        responseSize = response.str().length();
    }
    int flags = fcntl(client->getClientSock()->getFdSock(), F_GETFL, 0);
    if (flags & O_NONBLOCK) {
        std::cout << "The socket is in non-blocking mode" << std::endl;
    } else {
        std::cout << "The socket is in blocking mode" << std::endl;
    }
    size_t byteCount = send_all(client->getClientSock()->getFdSock(), response.str().c_str(), responseSize, MSG_DONTWAIT);
    if ((int) byteCount == SOCKET_ERROR) {
        std::cout << RED << "Error: not sended 500" << RESET_COLOR << std::endl;
        this->_statusCode = 500;
        if (epoll_ctl(ws_ptr->getEpollFd(), EPOLL_CTL_DEL, client->getClientSock()->getFdSock(), &client->event()) == 0)
            return;
        if (close(client->getClientSock()->getFdSock()) != -1)
            return;
    } else if (byteCount == 0) {
        std::cout << YELLOW << "send " << byteCount << " byte" << RESET_COLOR << std::endl;
        if (this->_statusCode == 0)
            this->_statusCode = 204;
    } else if (byteCount == responseSize) {
        std::cout << GREEN << "send " << byteCount << " byte" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_complete = true;
    }
}

void Response::readFromFile(std::string path, Client *client) {
    struct stat infoFile;
    stat(path.c_str(), &infoFile);
    if (infoFile.st_size < 0) {
        std::cout << RED << "Error ini stat()" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }

    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()){
        std::cout << RED << "Error in file open" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }
    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    this->_body = body;
    this->_bodySize = body.length();
    this->_fileExtension = getFileExtension(path);
    if (!this->_body.empty()) {
        this->_statusCode = 200;// Close the file
        if(this->_bodySize > 4096)
            client->setChunked(true);
    }
    file.close();
}

void Response::isDirectory(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        std::cout << RED << "Error in stat()" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }
    if (path == "/") {
        this->_statusCode = 200;
        this->_body = "server is online";
        this->_bodySize = this->_body.length();
        return;
    }
    if (S_ISDIR(info.st_mode)) {
        this->_statusCode = 200;
        this->_body = "is a directory";
        this->_bodySize = this->_body.length();
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

void Response::set_return(bool bull) {
    this->return_ = bull;
}


bool Response::getReturn_() {
    return return_;
}

std::string Response::getLocation() {
    return _location;
}

void Response::setLocation(std::string &location) {
    this->_location = location;

}
