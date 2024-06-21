#include "../include/Request.hpp"

Request::Request()
{
	this->_timeStart=0;
	this->_cgi=false;
	this->_error=false;
	this->_ended=false;
    this->_isRoot=false;
	this->_body_size=0;
	this->_headerSize=0;
	this->_method="";
	this->_header="";
	this->_body="";
    this->_isBody=false;
    this->_content_type="";
    this->_mymeType="";
//	std::cout << "Request : Default Constructor Called" << std::endl;
}

Request::~Request()
{
    this->_timeStart=0;
    this->_cgi=false;
    this->_error=false;
    this->_ended=false;
    this->_isRoot=false;
    this->_isBody=false;
    this->_body_size=0;
    this->_headerSize=0;
    this->_method="";
    this->_header="";
    this->_body="";
    this->_content_type="";
    this->_mymeType="";
//	std::cout << "Request : Destructor Called" << std::endl;
}

Request::Request(Request const &obj)
{
//	std::cout << "Request :Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Request	&Request::operator= (const Request &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		this->_error=obj._error;
		this->_cgi=obj._cgi;
		this->_ended=obj._ended;
		this->_timeStart=obj._timeStart;
		this->_body_size=obj._body_size;
		this->_requestHeaders=obj._requestHeaders;
		this->_method=obj._method;
		this->_headerSize=obj._headerSize;
		this->_header=obj._header;
	}
	return (*this);
}

/*
 *receiveData: old prototype must be upgrated read the fd and put it on buffer
 */

void Request::receiveData(Client *client) {
    if(this->ended() && !client->get_not_complete()) {
        std::cout<<GREEN<<"data already received"<<RESET_COLOR<<std::endl;
        return;
    }
	char rcv_buffer[RCV_BUF_SIZE];
	memset(rcv_buffer,0,RCV_BUF_SIZE);
	int byteCount=(int)recv(client->getClientSock()->getFdSock(),rcv_buffer, RCV_BUF_SIZE,0);
	if(byteCount==0)
	{
		this->_ended=true;
		client->response()->set_status_code(204);
		return;
	}else if(byteCount<0)
	{
		client->response()->set_status_code(1);
		std::cout<<RED<<"Error: client couldn't read"<<RESET_COLOR<<std::endl;
		return;
	}
	else if(byteCount !=0) {
		std::cout<<GREEN<<"receive data, "<<byteCount<<" byte"<<RESET_COLOR<<std::endl;
		if(!client->get_not_complete())
        {
            checkTypeAndSize(rcv_buffer);
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
	if (method.find("GET") == 0 || method.find("POST") == 0 || method.find("DELETE") == 0 || method.find("HEAD") == 0)
        set_method(method);
    else
        this->_error = true;
    if(this->method()=="POST")
        this->_isBody=true;
    if(httpRequest.find("Connection")==std::string::npos)
        this->_connection="close";
    int headerEnd=httpRequest.find("\r\n\r\n");
    this->_headerSize = headerEnd + 4;
    this->_header=httpRequest.substr(0,headerEnd+4);
}

/*
 *parseRequest: parse http request all heather
 *necessary to fill information like hethere body method etc
 */

int Request::parseRequest(std::string httpRequest)
{
	std::string input= httpRequest;
	size_t pos = 0;
	int i = 0;
	while ((pos = input.find("\n")) != std::string::npos)
	{
		std::string line = input.substr(0, pos);
		input.erase(0, pos + 1);
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
	return 0;
}

void Request::fillRequest(std::string &httpRequest) {
	size_t methodEndPos = httpRequest.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	std::istringstream input(httpRequest);
	std::string firstLine;
	std::getline(input, firstLine);
	setUrlPathQuery(firstLine);
 	for(std::map<std::string, std::string>::iterator it = this->_requestHeaders.begin();it != this->_requestHeaders.end(); it++)
 	{
 		if(it->first=="Connection") {
 			it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
 			this->_connection=it->second;
 		}
         if(it->first=="Content-Length")
         {
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             this->_body_size=toInt(it->second);
             this->_isBody=true;
         }
         if(it->first=="Content-Type")
         {
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             this->_content_type=it->second;
         }
         if(it->first=="Filename")
         {
             it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
             this->_fileName=it->second;
         }
 		if(it->first=="Content-Disposition") {
 			std::string str = "filename=\"";
 			size_t fileNameStart=it->second.find(str);
			if(fileNameStart!=std::string::npos)
 				this->_fileName=it->second.substr(fileNameStart+str.length(), it->second.find(".",fileNameStart)-fileNameStart-str.length());
 			this->_extension=it->second.substr(it->second.find(".",fileNameStart)+1, it->second.find("\r",fileNameStart)-it->second.find(".",fileNameStart)-2);
 		}
 		if(it->first=="Content-Type1") {
 			it->second.erase(std::remove(it->second.begin(), it->second.end(), '\r'), it->second.end());
			this->_mymeType=it->second;
 		}
 	}
 }

void Request::setUrlPathQuery(std::string &url) {
    size_t methodEndPos = url.find(" ");
	if (methodEndPos == std::string::npos)
		this->_error= true;
	size_t urlStartPos = methodEndPos + 1;
	size_t urlEndPos = url.find(" ", urlStartPos);
	this->_requestURL=url.substr(urlStartPos, urlEndPos - urlStartPos);
	this->_query= getQueryFromHttpRequest(url);
    fillQueryMap(this->_query);
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
		return httpRequest.substr(queryStartPos + 1);
	}
	return httpRequest.substr(queryStartPos + 1, queryEndPos - queryStartPos - 1);
}

void Request::fillQueryMap(std::string &query) {
    std::vector<std::string> queryVector;
    queryVector=AKAftSplit(query, "&");
    for(std::vector<std::string>::iterator it = queryVector.begin();it != queryVector.end(); it++)
    {
        std::string line = *it;
        size_t equalPos = line.find_first_of('=');
        if (equalPos != std::string::npos)
        {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            this->_queryMap[key] = value;
        }
    }
}

void Request::buildBody(Client *client,char*  input, int size){

    static size_t len=0;
	if(client->get_not_complete())
		len+= size;
	else
		len = size - this->_headerSize;
    if(this->_content_type.find("multipart/form-data;")!=std::string::npos)
    {
        std::string temp(input, size);
        std::string boundary="boundary=";
        size_t bodyStart=temp.find("\r\n\r\n", this->_headerSize);
        if(client->get_not_complete())
            this->_body+=temp;
        else
            this->_body=temp.substr(bodyStart+4);
    }
    else if (this->_headerSize > 0 && this->_body_size > 0 && this->_body.length() == 0){
        this->_mymeType= this->_content_type;
        std::string temp(input, size);
        this->_body=temp.substr(this->_headerSize);
    }
    if(len==this->_body_size){
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

const std::map<std::string, std::string> &Request::getQueryMap() const {
    return _queryMap;
}

void Request::setQueryMap(const std::map<std::string, std::string> &queryMap) {
    _queryMap = queryMap;
}
