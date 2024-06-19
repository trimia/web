#include "../include/Request.hpp"

Request::Request()
{
	this->_timeStart=0;
	this->_cgi=false;
	this->_error=false;
	this->_ended=false;
	// this->_not_complete=false;
    this->_isRoot=false;
	// this->_isPathFileDir=true;
	this->_body_size=0;
	this->_headerSize=0;
	this->_method="";
	this->_header="";
	this->_body="";
    this->_isBody=false;
    this->_content_type="";
    this->_mymeType="";

	std::cout << "Request : Default Constructor Called" << std::endl;
}

Request::~Request()
{
    this->_timeStart=0;
    this->_cgi=false;
    this->_error=false;
    this->_ended=false;
    // this->_not_complete=false;
    this->_isRoot=false;
    this->_isBody=false;
    // this->_isPathFileDir=true;
    this->_body_size=0;
    this->_headerSize=0;
//    memset(&this->_method,0,this->_method.length());
//    memset(&this->_header,0,this->_header.length());
//    memset(&this->_body,0,this->_body.length());
//    memset(&this->_content_type,0,this->_content_type.length());
//    memset(&this->_mymeType,0,this->_mymeType.length());
    this->_method="";
    this->_header="";
    this->_body="";
    this->_content_type="";
    this->_mymeType="";

    // this->_timeStart=0;
	// this->_cgi=false;
	// this->_error=false;
	// this->_ended=false;
	// this->_not_complete=false;
	std::cout << "Request : Destructor Called" << std::endl;
}

Request::Request(Request const &obj)
{
	std::cout << "Request :Copy Constructor Called" << std::endl;
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
		// this->_not_complete=obj._not_complete;
		this->_timeStart=obj._timeStart;
		this->_body_size=obj._body_size;
		this->_requestHeaders=obj._requestHeaders;
		this->_method=obj._method;
		this->_headerSize=obj._headerSize;
		this->_header=obj._header;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}

/*
 *receiveData: old prototype must be upgrated read the fd and put it on buffer
 */

void Request::receiveData(Client *client) {
    std::cout<<MAGENTA<<"receiving data"<<std::endl;
    if(this->ended() && !client->get_not_complete()) {
        std::cout<<GREEN<<"data already received"<<RESET_COLOR<<std::endl;
        return;
    }
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
		std::cout<<GREEN<<"read 0 byte"<<RESET_COLOR<<std::endl;
		this->_ended=true;
		client->response()->set_status_code(204);
		return;
	}else if(byteCount<0)
	{
		client->response()->set_status_code(1);
		// this->_error=true;
		std::cout<<RED<<"Error: client couldn't read"<<RESET_COLOR<<std::endl;
		return;
	}
	else if(byteCount !=0) {
//         for (const auto &item: rcv_buffer){
//             std::cout << item;
//         }
		std::cout<<CYAN<<"buffer:"<<std::endl<<rcv_buffer<<RESET_COLOR<<std::endl;
		std::cout<<GREEN<<"receive data, "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		if(!client->get_not_complete())
        {
            checkTypeAndSize(rcv_buffer);
//			std::cout<<CYAN<<"header :"<<this->_header<<RESET_COLOR<<std::endl;
            parseRequest(rcv_buffer);
            fillRequest(this->_header);
        }
        if(this->is_body()){
            buildBody(client,rcv_buffer,byteCount);
        } else if(!client->get_not_complete())
            this->_ended=true;
		client->set_connection(this->connection());
        if(client->is_location())
        {
            Location temp;

            temp=client->getTempLocation(client->locations(),this->request_url());
            if( !temp.clientMaxBodySize().empty() && this->_body_size>(size_t)toInt(temp.clientMaxBodySize()))
            {
                this->_ended=true;
                client->response()->set_status_code(413);
                return;
            }
        }
        if((client->getClientMaxBodySize()!=0 && this->_body_size > client->getClientMaxBodySize()))
        {
            this->_ended=true;
            client->response()->set_status_code(413);
            return;
        }
        if(this->_headerSize!=0 && this->_headerSize>MAX_HEADER_SIZE)
        {
            this->_ended=true;
            client->response()->set_status_code(431);
            return;
        }

        printCharsAndSpecialChars(this->http_version());
		if(this->http_version()!="HTTP/1.1")
        {
            client->response()->set_status_code(505);
            this->_ended=true;
        }
		if(this->error())
		{
			std::cout<<RED<<"Error: bad request"<<RESET_COLOR<<std::endl;
            this->_ended=true;
			client->response()->set_status_code(400);
			return;
		}
		return;
		//understand if we want to register the parsed request in the client
		// client->request()->setRequestHeaders(client->request()->parseRequest(rcv_buffer));
		// this->checktype(rcv_buffer);
		// return byteCount;
	}
}



void Request::checkTypeAndSize(std::string httpRequest) {
	size_t methodEndPos = httpRequest.find(" ");
	if (methodEndPos == std::string::npos)
    {
		this->_error= true;
		return ;
	}
	std::istringstream input(httpRequest);
	std::string method;
	input>> method;
	// std::cout<<YELLOW<<"checktype method :"<<method<<RESET_COLOR<<std::endl;
	if (method.find("GET") == 0 || method.find("POST") == 0 || method.find("DELETE") == 0 || method.find("HEAD") == 0)
    {
		set_method(method);
	} else
		this->_error = true;
    if(this->method()=="POST"){
        this->_isBody=true;
    }
//	std::cout<<YELLOW<<"checktype method :"<<this->method()<<RESET_COLOR<<std::endl;
    if(httpRequest.find("Connection")==std::string::npos)
        this->_connection="close";
    int headerEnd=httpRequest.find("\r\n\r\n");
    this->_headerSize = headerEnd + 4;
    this->_header=httpRequest.substr(0,headerEnd+4);
//    std::cout<<YELLOW<<"header size :"<<this->_headerSize<<" header :"<<this->_header.length()<<"headerEnd :"<<headerEnd<<RESET_COLOR<<std::endl;
//    std::cout<<YELLOW<<"header :"<<this->_header<<RESET_COLOR<<std::endl;
}

/*
 *parseRequest: parse http request all heather
 *necessary to fill information like hethere body method etc
 */

int Request::parseRequest(std::string httpRequest)
{
	std::cout<<YELLOW<<"parseRequest"<<RESET_COLOR<<std::endl;
	std::string input= httpRequest;
	size_t pos = 0;
	int i = 0;
	while ((pos = input.find("\n")) != std::string::npos)
	{
		std::string line = input.substr(0, pos);
		// std::cout<<YELLOW<<"line :"<<line<<RESET_COLOR<<std::endl;
		// printCharsAndSpecialChars(line);
		input.erase(0, pos + 1);
		//TODO check if is necessary a better way to escape from while
		if ((line=="\r" && this->method()!="POST")||(line=="\r" && i==1))
			break;
		if (line=="\r" && this->method()=="POST"){
			i=1;
		}
		size_t colonPos = line.find_first_of(':');
		if (colonPos != std::string::npos)
		{
			std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2); // Skip the colon and space
            {
                if (this->_requestHeaders.find(key) == this->_requestHeaders.end())
                    this->_requestHeaders[key] = value;
                else
                    this->_requestHeaders[key + "1"] = value;
            }

        }
	}

//	 print for debug
//	 for (const auto& pair : this->_requestHeaders) {
//	     std::cout<<BLUE << pair.first << " first : second " << pair.second << RESET_COLOR<<std::endl;
//	 }
	return 0;
}

void Request::fillRequest(std::string &httpRequest) {
	size_t methodEndPos = httpRequest.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	std::istringstream input(httpRequest);
	std::string firstLine;
	std::getline(input, firstLine);
	// std::cout<<YELLOW<<"fillRequest firstLine :"<<firstLine<<RESET_COLOR<<std::endl;
	setUrlPathQuery(firstLine);
 	for(std::map<std::string, std::string>::iterator it = this->_requestHeaders.begin();it != this->_requestHeaders.end(); it++)
 	{
 		if(it->first=="Connection") {
 			it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
 			this->_connection=it->second;
 		}
         if(it->first=="Content-Length")
         {
             std::cout<<YELLOW<<"fillRequest Content-Length :"<<it->second<<RESET_COLOR<<std::endl;
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             printCharsAndSpecialChars(it->second);
             this->_body_size=toInt(it->second);
             this->_isBody=true;
         }
         if(it->first=="Content-Type")
         {
             std::cout<<YELLOW<<"fillRequest Content-Type :"<<it->second<<RESET_COLOR<<std::endl;
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             printCharsAndSpecialChars(it->second);
             this->_content_type=it->second;
         }
         if(it->first=="Filename")
         {
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             printCharsAndSpecialChars(it->second);
             this->_fileName=it->second;
         }
 		if(it->first=="Content-Disposition") {
 			std::cout<<YELLOW<<"fillRequest Content-Disposition :"<<RESET_COLOR<<std::endl;
 			std::string str = "filename=\"";
 			size_t fileNameStart=it->second.find(str);
			if(fileNameStart!=std::string::npos)
 				this->_fileName=it->second.substr(fileNameStart+str.length(), it->second.find(".",fileNameStart)-fileNameStart-str.length());
 			printCharsAndSpecialChars(this->_fileName);
 			this->_extension=it->second.substr(it->second.find(".",fileNameStart)+1, it->second.find("\r",fileNameStart)-it->second.find(".",fileNameStart)-2);
 			printCharsAndSpecialChars(this->_extension);
 		}
 		if(it->first=="Content-Type1") {
 			it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
			this->_mymeType=it->second;
 			printCharsAndSpecialChars(this->_mymeType);
 		}
 	}
 }

void Request::setUrlPathQuery(std::string &url) {
    std::cout<<YELLOW<<"setUrlPathQuery url :"<<url<<RESET_COLOR<<std::endl;
    size_t methodEndPos = url.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	size_t urlStartPos = methodEndPos + 1;
	size_t urlEndPos = url.find(" ", urlStartPos);
	this->_requestURL=url.substr(urlStartPos, urlEndPos - urlStartPos);
	this->_query= getQueryFromHttpRequest(url);
	std::cout<<RED<<"requestURL :"<<this->_requestURL<<std::endl;
	size_t endLinePos = url.find("\r", urlEndPos);
	if (endLinePos == std::string::npos) {
		this->_HTTPVersion=url.substr(urlEndPos + 1);
	} else {
		this->_HTTPVersion=url.substr(urlEndPos + 1, endLinePos - urlEndPos - 1);
	}
	int pathEndPos=0;
	int n = url.find("?");
	if (n >0)
		pathEndPos = n;
	else pathEndPos = urlEndPos;
	this->_path_file=url.substr(urlStartPos, pathEndPos - urlStartPos);
	if(this->path_file()=="/")
		this->_isRoot=true;
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

void Request::buildBody(Client *client,char*  input, int size){

    std::cout<<YELLOW<<"buildBody"<<RESET_COLOR<<std::endl;
    std::cout<<YELLOW<<"client socket fd: "<<client->client_sock()->getFdSock()<<RESET_COLOR<<std::endl;
    static size_t len=0;
	if(client->get_not_complete())
		len+= size;
	else
		len = size - this->_headerSize;
    std::cout<<YELLOW<<"content type :"<<this->_content_type<<RESET_COLOR<<std::endl;
    if(this->_content_type.find("multipart/form-data;")!=std::string::npos)
    {
    	std::cout<<YELLOW<<"multipart/form-data"<<RESET_COLOR<<std::endl;
        std::string temp(input, size);
        std::string boundary="boundary=";
//        size_t boundaryPos=temp.find(boundary);
//        if(boundaryPos==std::string::npos)
//        {
//            this->_ended=true;
//            client->response()->set_status_code(400);
//            return;
//        }
        size_t bodyStart=temp.find("\r\n\r\n", this->_headerSize);
        if(client->get_not_complete())
            this->_body+=temp;
        else
            this->_body=temp.substr(bodyStart+4);

//        std::cout<<GREEN<<std::endl;
//        for (const auto &item: this->_body){
//            printsingleCharAndSpecialChar(item);
//        }
//        std::cout<<RESET_COLOR<<std::endl;

//        boundaryPos+=boundary.length();
//        size_t boundaryEndPos=temp.find("\r\n", boundaryPos);
//        std::string boundaryString=temp.substr(boundaryPos, boundaryEndPos - boundaryPos);
//        std::vector<std::string> parts = AKAftSplit(this->_body, boundaryString+"--");
//        for(std::vector<std::string>::iterator it=parts.begin();it!=parts.end();it++)
//        {
//        	this->_body+=*it;
//            //WIP
//        }
    }
    else if (this->_headerSize > 0 && this->_body_size > 0 && this->_body.length() == 0){
        this->_mymeType= this->_content_type;
        std::string temp(input, size);
        this->_body=temp.substr(this->_headerSize);
        std::cout<<YELLOW<<"body length :"<<this->_body.length()<<RESET_COLOR<<std::endl;
    }
    std::cout<<BLUE<<"len : "<<len<<" body size : "<<this->_body_size<<RESET_COLOR<<std::endl;
    if(len==this->_body_size){
        std::cout<<CYAN<<"end of body"<<RESET_COLOR<<std::endl;
        this->_ended=true;
        client->set_not_complete(false);

    } else if(!client->get_not_complete())
		client->set_not_complete(true);
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
//
// bool Request::complete() const {
// 	return _not_complete;
// }
//
// void Request::set_complete(bool complete) {
//     _not_complete = complete;
// }

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

std::string Request::http_version() const {
	return _HTTPVersion;
}

void Request::set_http_version(const std::string &http_version) {
	_HTTPVersion = http_version;
}

bool Request::is_body() const {
	return _isBody;
}

void Request::set_is_body(bool is_body) {
	_isBody = is_body;
}

bool Request::is_query() const {
	return _isQuery;
}

void Request::set_is_query(bool is_query) {
	_isQuery = is_query;
}

std::string Request::request_url() const {
	return _requestURL;
}

void Request::set_request_url(const std::string &request_url) {
	_requestURL = request_url;
}

std::string Request::path_file() const {
	return _path_file;
}

void Request::set_path_file(const std::string &path_file) {
	_path_file = path_file;
}

bool Request::is_root() const {
	return _isRoot;
}

void Request::set_is_root(bool is_rooth) {
	_isRoot = is_rooth;
}

std::string Request::connection() const {
	return _connection;
}

void Request::set_connection(std::string connection) {
	_connection = connection;
}

size_t Request::header_size(){
	return _headerSize;
}

void Request::set_header_size(size_t header_size) {
	_headerSize = header_size;
}

std::string & Request::file_name() {
	return _fileName;
}

void Request::set_file_name(std::string file_name) {
	_fileName = file_name;
}

std::string & Request::getExtension() {
	return _extension;
}

void Request::set_extension(std::string extension) {
	_extension = extension;
}

std::string & Request::getMyme_type() {
	return _mymeType;
}

void Request::set_myme_type(std::string myme_type) {
	_mymeType = myme_type;
}

// bool & Request::getNot_complete() {
// 	return _not_complete;
// }
//
// void Request::set_not_complete(bool not_complete) {
// 	_not_complete = not_complete;
// }

inline std::map<std::string, std::string> Request::request_headers() const {
	return _requestHeaders;
}

inline void Request::set_request_headers(const std::map<std::string, std::string> &request_headers) {
	_requestHeaders = request_headers;
}

std::string Request::http_message(){
	return _header;
}

inline void Request::set_http_message(const std::string &http_message) {
    _header = http_message;
}

size_t Request::getClientMaxBodySize() const {
    return _clientMaxBodySize;
}

void Request::setClientMaxBodySize(size_t clientMaxBodySize) {
    _clientMaxBodySize = clientMaxBodySize;
}

const std::string &Request::getContentType() const {
    return _content_type;
}

void Request::setContentType(const std::string &contentType) {
    _content_type = contentType;
}

const std::string &Request::getBody() const {
    return _body;
}

void Request::setBody(const std::string &body) {
    _body = body;
}
