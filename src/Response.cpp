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
//    this->_location = new Location();

    std::cout << "Response : Default Constructor Called" << std::endl;
}

Response::~Response() {
    std::cout << "Response : Destructor Called" << std::endl;
//    delete this->_location;
//    memset(&this->_header, 0, this->_headerSize);
//    memset(&this->_body, 0, this->_bodySize);
//    memset(&this->_content_type, 0, this->_content_type.length());
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
    buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
                            getMimeType(this->file_extension()), this->body_size());

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
    std::cout<<CYAN<<"root: "<<this->root()<<RESET_COLOR<<std::endl;
    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    std::cout << YELLOW << "root: " << this->root() << RESET_COLOR << std::endl;
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;
    if (!client->is_location() && client->request()->path_file() == "/") {
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
    } else if (!client->cgi())
        readFromFile(this->root() + client->request()->path_file(),client);
    else
    std::cout << YELLOW << "path file: " << this->root() + client->request()->path_file() << RESET_COLOR << std::endl;

    // buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
    //                         getMimeType(this->file_extension()), this->body_size());
    return;
}
void Response::postMethod(Client *client) {
    if(client->get_not_complete())
        return;
    std::cout << YELLOW << "POSTMETHOD" << RESET_COLOR << std::endl;
    // std::cout<<BLUE<<"body size :"<<client->request()->body_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"header size :"<<client->request()->header_size()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"time start :"<<client->request()->time_start()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"error :"<<client->request()->error()<<RESET_COLOR<<std::endl;
    // // std::cout<<BLUE<<"cgi :"<<client->request()->cgi()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"ended :"<<client->request()->ended()<<RESET_COLOR<<std::endl;
    // std::cout<<BLUE<<"method :"<<client->request()->method()<<RESET_COLOR<<std::endl;
    // checkRequest(client);
    std::string str;

    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    std::string path;
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;
    struct stat infoFile;
    if(client->request()->path_file() == "/") {
        path=this->_root+"/upload";
        std::cout<<"if " << YELLOW << "stat path file: " << path << RESET_COLOR << std::endl;
        // stat((this->_root+path).c_str(), &infoFile);

    }
    else {
        path=this->_root+client->request()->path_file();
        std::cout<<"else "  << YELLOW << "stat path file: " << path << RESET_COLOR << std::endl;
        // stat((this->_root+client->request()->path_file()).c_str(), &infoFile);
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
            // client->request()->set_file_name();
            client->setTempfilen(client->getTempfilen()+1);
        }else
            str=path+"/"+client->request()->file_name()+"."+client->request()->getExtension();
        std::cout << YELLOW << "str: " << str << RESET_COLOR << std::endl;
        std::ofstream file(str.c_str(), std::ios::app | std::ios::binary);

//        print debug
        std::cout<<"body"<< client->request()->getBody().length()<<GREEN<<std::endl;
        for (const auto &item: client->request()->getBody()){
            printsingleCharAndSpecialChar(item);
        }
        std::cout<<RESET_COLOR<<std::endl;



        file.write(client->request()->getBody().c_str(), client->request()->getBody().size());
        if(file.fail())
        {
            std::cout << RED << "file write error" << RESET_COLOR << std::endl;
            this->_statusCode = 500;
            return;
        }
        file.close();
        this->_statusCode = 201;
        }else {
            std::cout << RED << "stat() error" << RESET_COLOR << std::endl;
            this->_statusCode = 409;
            return;
    }


    // buildHttpResponseHeader(client->request()->http_version(), StatusString(this->status_code()),
    //                         getMimeType(this->file_extension()), this->body_size());

}


void Response::deleteMethod(Client *client) {
    std::cout <<CYAN<< "deleteMethod" <<RESET_COLOR <<std::endl;

    if (client->is_location())
        handleLocation(client);
    if(this->error()||this->ready_to_send())
        return;
    if (this->root().empty() || this->root() == "/")
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }

    std::string uri=client->request()->path_file();
    std::string fileName=uri.substr(client->request()->path_file().find_last_of("/"));
    std::string path=uri.substr(0,client->request()->path_file().find_last_of("/"));
    std::cout<<"path "<<path<<std::endl;
//    std::cout<<"file name"<<fileName<<std::endl;
    std::string dirPath = this->_root + path;
    std::cout<<"dir path"<<dirPath<<std::endl;
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
        this->_root = "/www";
    if(!client->is_location())
        checkPath(client);
    if (this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;
    if (!client->is_location() && client->request()->path_file() == "/") {
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
        readFromFile(this->root() + client->request()->path_file(), client);
    std::cout << YELLOW << "path file: " << this->root() + client->request()->path_file() << RESET_COLOR << std::endl;
    return;
}

void Response::checkPath(Client *client) {
    std::cout << CYAN << "checkPath" << RESET_COLOR << std::endl;
    std::cout << YELLOW << "root: " << this->_root << RESET_COLOR << std::endl;
    size_t rootPos=client->request()->path_file().find(this->_root);
    std::cout<<"root pos: "<<rootPos<<std::endl;
    if(rootPos != std::string::npos){
//        std::cout<<"pre   " << CYAN << "path file: " << client->request()->path_file() <<" path file lenght "<<client->request()->path_file().length()<< RESET_COLOR << std::endl;
        std::string temp;
        temp=client->request()->path_file().erase(rootPos, this->_root.length());
//        std::cout <<"temp   "<< CYAN << temp <<" temp lenght "<<temp.length()<< RESET_COLOR << std::endl;
        client->request()->set_path_file(temp);
//        std::cout <<"post   "<< CYAN << "path file: " << client->request()->path_file()  <<" path file lenght "<<client->request()->path_file().length()<< RESET_COLOR << std::endl;

    }
    if(client->request()->path_file().find_last_of("/")==client->request()->path_file().length()){
        std::string temp;
        temp=client->request()->path_file().substr(0,client->request()->path_file().length()-1);
        client->request()->set_path_file(temp);
    }

}

void Response::handleLocation(Client *client) {

    std::cout << CYAN << "handleLocation" << RESET_COLOR << std::endl;
//     std::cout<<YELLOW<<"fit best location"<<RESET_COLOR<<std::endl;
//     std::cout << YELLOW<<"path file:" <<client->request()->path_file() << RESET_COLOR << std::endl;
//     if (!client->locations().empty())
//         for (std::vector<Location>::iterator it1 = client->locations().begin(); it1 != client->locations().end(); ++it1) {
//             std::cout << BLUE << "LOC PATH : " << it1->getPath() << RESET_COLOR << std::endl;
//             if (!it1->getMethods().empty()) {
//                 std::cout << BLUE << "LOC METHODS -> " << it1->getMethods()[0] << " : " << it1->getMethods()[1]
//                           << RESET_COLOR << std::endl;
//             }
//         }
//     else
//         std::cout << RED << "LOCATIONS EMPTY" << RESET_COLOR << std::endl;
    // // Itera attraverso le posizioni definite nel server
    Location bestMatch;
    size_t bestMatchLenght = 0;
    if (!client->locations().empty())
        for (std::vector<Location>::iterator it = client->locations().begin(); it != client->locations().end(); ++it) {
            if (client->request()->path_file().find(it->getPath()) != std::string::npos && it->getPath().length() > bestMatchLenght) {
                bestMatch = *it;
                bestMatchLenght = it->getPath().length();
                 // std::cout << GREEN << "BEST MATCH PATH : " << bestMatch.getPath() << RESET_COLOR << std::endl;
                // std::cout << GREEN << "BEST MATCH METHOD -> " << bestMatch->getMethods()[0] << RESET_COLOR << std::endl;
            }
        }
//    std::cout << std::boolalpha << GREEN << "BEST MATCH getIsCgi() : " << bestMatch.getIsCgi() << RESET_COLOR << std::endl;
//    if(bestMatch.getIsCgi()) { ////////////// is it the right moment to check the CGI ?
//        Cgi cgi(client->request());
//		cgi.executeCgi(client);
//        std::cout << BLUE << client->response()->_body << RESET_COLOR << std::endl;
//    }
    std::cout << GREEN << "BEST MATCH PATH : " << bestMatch.getPath() << RESET_COLOR << std::endl;
    if (!bestMatch.getMethods().empty() && !bestMatch.allowMethod(client->request()->method())) {
        std::cout << RED << "method not allowed" << RESET_COLOR << std::endl;
        this->_error = true;
        this->_statusCode = 405;
        return;
    }
    std::cout<<CYAN<<"root: "<<this->root()<<RESET_COLOR<<std::endl;
    if (bestMatch.isIsRoot() &&(this->root() == "/" || bestMatch.root().empty()))
        this->_root = "/www";
    else if(bestMatch.isIsRoot())
        this->_root = bestMatch.root();
    std::cout<<CYAN<<"root: "<<this->root()<<RESET_COLOR<<std::endl;
    checkPath(client);
    if (this->_root.find(".") == std::string::npos) {
        this->_root = "." + this->root();
    }
    // std::cout << YELLOW << "root: " << this->root() << RESET_COLOR << std::endl;
    std::cout << YELLOW << "path file: " << client->request()->path_file() << RESET_COLOR << std::endl;

    if (!bestMatch.alias().empty()){
        std::cout << CYAN << "alias" << bestMatch.alias() << RESET_COLOR << std::endl;
        size_t locationPathPos = client->request()->path_file().find(bestMatch.getPath());
        if ( locationPathPos  != std::string::npos){
            std::cout<<"location path pos: "<<locationPathPos<<std::endl;
            std::cout<<"pre   " << CYAN << "path file: " << client->request()->path_file() <<" path file lenght "<<client->request()->path_file().length()<< RESET_COLOR << std::endl;
            std::string temp;
            temp=client->request()->path_file().replace(locationPathPos, bestMatch.getPath().length(), bestMatch.alias());
            client->request()->set_path_file(temp);
//            std::stringstream ss;
//            ss << client->request()->path_file().substr(0, locationPathPos);
//            ss << bestMatch.alias();
//            ss << client->request()->path_file().substr(locationPathPos + bestMatch.getPath().length());
//            client->request()->set_path_file(ss.str());
            std::cout <<"post   "<< CYAN << "path file: " << client->request()->path_file()  <<" path file lenght "<<client->request()->path_file().length()<< RESET_COLOR << std::endl;
        }
    }
    // std::cout << CYAN << "location autoindex: " << bestMatch.getAutoIndex() << RESET_COLOR << std::endl;
    if (bestMatch.getAutoIndex() && bestMatch.autoIndex(this->root() + client->request()->path_file())) {
        if (bestMatch.getIsCgi())
            return;
        std::cout << CYAN << "autoindex" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_body = bestMatch.generateDirectoryListing( client->request()->path_file(),this->root());
        this->_bodySize = this->_body.length();
        this->_content_type = "html";
        this->_readyToSend = true;
        return;
    }
    if (!bestMatch.index().empty() && !bestMatch.getIsCgi())
    {
        std::cout << CYAN << "index" << RESET_COLOR << std::endl;
        return readFromFile(bestMatch.index(),client);

    }
    if (!bestMatch.getReturn().empty() && !bestMatch.getIsCgi()) {
        std::cout << CYAN << "return" << RESET_COLOR << std::endl;
        std::vector<std::string> return_ = bestMatch.getReturn();
        for (std::vector<std::string>::iterator it = return_.begin(); it != return_.end(); ++it) {
            std::cout << YELLOW << "return" << *it << RESET_COLOR << std::endl;
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
    std::cout << std::boolalpha << GREEN << "BEST MATCH getIsCgi() : " << bestMatch.getIsCgi() << RESET_COLOR << std::endl;
    if(bestMatch.getIsCgi())
    {
        //TODO handle cgi
        Cgi cgi(client->request());
        cgi.executeCgi(client);
        client->set_cgi(true);
        std::cout << BLUE << client->response()->_body << RESET_COLOR << std::endl;
    }

//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(0) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(1) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(2) << RESET_COLOR << std::endl;
//    std::cout << YELLOW << "location return: " << this->location()->getReturn().at(3) << RESET_COLOR << std::endl;
//        return readFromFile(this->location().root()+client->request()->path_file());
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
    std::cout << CYAN << "buildRedirectResponseHeader" << RESET_COLOR << std::endl;
    std::cout << CYAN << "code: " << statusText << RESET_COLOR << std::endl;
    std::ostringstream header;
    header << httpVersion << " " << statusText << "\r\n";
    header << "Location: " << location << "\r\n";
    header << "\r\n";  // End of header
    this->_headerSize = header.str().length();
//    this->_readyToSend = true;
    std::cout << YELLOW << "header: " << header.str() << RESET_COLOR << std::endl;
    this->_header = header.str();
}

static size_t send_all(int socket, const char* buffer, size_t length, int flags) {
    size_t total_bytes_sent = 0;
    while (total_bytes_sent < length) {
        size_t bytes_sent = send(socket, buffer + total_bytes_sent, length - total_bytes_sent, flags);
        if ((int)bytes_sent == -1) {
            // An error occurred
            return -1;
        }
        total_bytes_sent += bytes_sent;
    }
    return total_bytes_sent;
}

void Response::sendData(Client *client) {
    std::cout << MAGENTA << "send data complete: " << this->complete() << RESET_COLOR << std::endl;
    std::cout << MAGENTA << "send data complete: " << client->response()->complete() << RESET_COLOR << std::endl;
    if (this->complete())
        return;

    if(this->status_code()>=400 && this->status_code()<=505)
    {
//        int temp= this->status_code();
        client->response()->readFromFile(getErrorPages(client->response()->status_code()),client);
//        client->response()->set_status_code(200);
        client->response()->buildHttpResponseHeader(client->request()->http_version(), StatusString(client->response()->status_code()),
                                                    getMimeType(client->response()->file_extension()), client->response()->body_size());
        std::cout << RED << "error" << RESET_COLOR << std::endl;
//        this->_readyToSend = true;
    }

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
    std::ostringstream response;
    size_t responseSize = 0;

//    if(!client->isChunked()) {
//        size_t responseSize = this->header().length() + this->body_size() + 4;
//        char *response = new char[responseSize];
//        memset(response, 0, responseSize);
//        for (size_t i = 0; i < this->header().length(); i++)
//            response[i] = this->header().at(i);
//        for (size_t i = 0; i < this->body_size(); i++)
//            response[this->header_size() + i] = this->body().at(i);
//        response[this->header_size() + this->body_size()] = '\r';
//        response[this->header_size() + this->body_size() + 1] = '\n';
//        response[this->header_size() + this->body_size() + 2] = '\r';
//        response[this->header_size() + this->body_size() + 3] = '\n';
//    } else
    std::cout<<std::boolalpha << YELLOW << "is chunked: " << client->isChunked() << RESET_COLOR << std::endl;
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
        response << "\r\n";  // End of header

        // Add the body in chunks
        size_t chunkSize = 4096;  // Size of each chunk
        for (size_t i = 0; i < this->_body.length(); i += chunkSize) {
            // Calculate the size of the current chunk
            size_t currentChunkSize = std::min(chunkSize, this->_body.length() - i);

            // Add the size of the current chunk
            response << std::hex << currentChunkSize << "\r\n";

            // Add the current chunk
            response << this->_body.substr(i, currentChunkSize);

            // Add the chunk end marker
            response << "\r\n";
        }

        // Add the end of the message
        response << "0\r\n\r\n";
        responseSize = response.str().length();
    }

    std::cout << YELLOW << "response: " << response.str().c_str() << " response size:" << responseSize << RESET_COLOR << std::endl;
    std::cout << YELLOW << " header response: " <<client->response()->header()<< " response size:" << responseSize << RESET_COLOR << std::endl;

//    fcntl(response, F_SETFL, O_NONBLOCK);

    int flags = fcntl(client->getClientSock()->getFdSock(), F_GETFL, 0);

// Check if the O_NONBLOCK flag is set
    if (flags & O_NONBLOCK) {
        std::cout << "The socket is in non-blocking mode" << std::endl;
        // The socket is in non-blocking mode
    } else {
        std::cout << "The socket is in blocking mode" << std::endl;
        // The socket is in blocking mode
    }

//    std::cout << YELLOW << "response: " << response.str().c_str() << " response size:" << responseSize << RESET_COLOR << std::endl;
    //TODO https://linux.die.net/man/2/send see which falgs to use or if 0 is ok
    size_t byteCount = send_all(client->getClientSock()->getFdSock(), response.str().c_str(), responseSize, MSG_DONTWAIT);
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
        // this->_not_complete = true;
        // return error handling
    } else if (byteCount == responseSize) {
        std::cout << GREEN << "send " << byteCount << " byte" << RESET_COLOR << std::endl;
        this->_statusCode = 200;
        this->_complete = true;
    }
    // this->_not_complete = false;
}

void Response::readFromFile(std::string path, Client *client) {
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
    if (!file.is_open()){
        std::cout << RED << "file open error" << RESET_COLOR << std::endl;
        this->_statusCode = 404;
        return;
    }
    // Read the file into a string
    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    this->_body = body;
//    std::cout << CYAN << "body: " << this->_body << RESET_COLOR << std::endl;
    this->_bodySize = body.length();
    this->_fileExtension = getFileExtension(path);
    std::cout << RED << std::endl;
    printCharsAndSpecialChars(this->file_extension());
    std::cout << RESET_COLOR << std::endl;
//    std::cout<<CYAN<<"body size: "<<this->_bodySize<<"body: "<<RESET_COLOR<<this->_body<<std::endl;
    if (!this->_body.empty()) {
        this->_statusCode = 200;// Close the file
        if(this->_bodySize > 4096)
            client->setChunked(true);
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

//Location *Response::location() const {
//    return _location;
//}
//
//void Response::set_location(Location *location) {
//    std::cout << "Response : set_location" << std::endl;
//    std::cout << "location path" << location->getPath() << std::endl;
//    std::cout << "location method" << location->getMethods()[0]<<location->getMethods()[1] << std::endl;
//    _location = location;
//}
//

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

//
//void Response::pippo(Client *client) {
//    // ...
//
//    // Add the "Transfer-Encoding: chunked" header
//    std::ostringstream response;
//    response.clear();
//    response << httpVersion << " " << statusText << "\r\n";
//    response << "Content-Type: " << contentType << "\r\n";
//    response << "Transfer-Encoding: chunked\r\n";
//    response << "\r\n";  // End of header
//
//    // Add the body in chunks
//    size_t chunkSize = 4096;  // Size of each chunk
//    for (size_t i = 0; i < this->_body.length(); i += chunkSize) {
//        // Calculate the size of the current chunk
//        size_t currentChunkSize = std::min(chunkSize, this->_body.length() - i);
//
//        // Add the size of the current chunk
//        response << std::hex << currentChunkSize << "\r\n";
//
//        // Add the current chunk
//        response << this->_body.substr(i, currentChunkSize);
//
//        // Add the chunk end marker
//        response << "\r\n";
//    }
//
//    // Add the end of the message
//    response << "0\r\n\r\n";
//
//    // Send the entire message
//    send_all(client->getClientSock()->getFdSock(), response.str().c_str(), response.str().length(), 0);
//}