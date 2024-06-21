#include "../include/Location.hpp"

Location::Location()
{
//	std::cout << "Location : Default Constructor Called" << std::endl;
    this->_path = "";
    this->_root = "";
    this->_isRoot = false;
    this->_autoindex = false;
    this->_isCgi=false;
    this->_index = "";
    this->_methods.reserve(0);
    this->_alias = "";
    this->_clientMaxBodySize = "";
    this->_return.reserve(0);
    this->_cgiExt.reserve(0);
}

Location::~Location()
{
//	std::cout << "Location : Destructor Called" << std::endl;
}

Location::Location(Location const &obj)
{
//	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj) {
	    this->_path=obj._path;
	    this->_root=obj._root;
        this->_isRoot=obj._isRoot;
	    this->_autoindex=obj._autoindex;
	    this->_index=obj._index;
        this->_isCgi = obj._isCgi;
	    this->setMethods(obj._methods);
        this->setReturn(obj._return);
	    this->_alias=obj._alias;
	    this->_clientMaxBodySize=obj._clientMaxBodySize;
	}
}

Location	&Location::operator= (const Location &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj)
    {
        this->_path=obj._path;
        this->_root=obj._root;
        this->_isRoot=obj._isRoot;
        this->_autoindex=obj._autoindex;
        this->_index=obj._index;
        this->setMethods(obj._methods);
        this->setReturn(obj._return);
        this->_isCgi=obj._isCgi;
        this->_alias=obj._alias;
        this->_clientMaxBodySize=obj._clientMaxBodySize;
    }
    return (*this);
}

bool Location::allowMethod(std::string method) {
    std::vector<std::string> methods = this->getMethods();
    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (it->compare(method) == 0)
            return true;
    }
    return false;
}

bool Location::autoIndex(std::string path){
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        std::cout << RED << "stat() error" << RESET_COLOR << std::endl;
        return false;
    }
    if(S_ISDIR(info.st_mode)){
        return true;
    }
    return false;
}

std::string Location::generateDirectoryListing(std::string path,std::string root){
    DIR* dir;
    struct dirent* ent;
    std::ostringstream html;
    std::string temp="";
    html << "<html><body><ul>";

    if ((dir = opendir((root+path).c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(std::string(ent->d_name)==".."){
                temp.clear();
                if(path=="/"||temp=="/"){
                    temp = "";
                }
                else{
                    temp=path;
                    temp.erase(path.find_last_of("/")+1,path.length() - path.find_last_of("/"));
                    temp=temp.substr(path.find_last_of("/"));
                }
                if(temp=="/"){
                    temp="";
                }

            } else{
                temp = ent->d_name;
            }
            if(path=="/"){
                path="";
            }
            std::string filePath = path + "/" + temp;
            if(path==""){
                path ="/";
            }
            html << "<li><a href=\"" << filePath << "\">" << ent->d_name << "</a></li>";
        }
        closedir(dir);
    } else {
        return "";
    }

    html << "</ul></body></html>";

    return html.str();
}

/*
 *			getter & setter
 */

std::string & Location::root() {
    return _root;
}

bool & Location::getAutoIndex() {
    return _autoindex;
}

std::string & Location::index() {
    return _index;
}

std::string & Location::alias() {
    return _alias;
}

void Location::set_alias(std::string alias) {
    _alias = alias;
}

std::basic_string<char> & Location::clientMaxBodySize() {
    return _clientMaxBodySize;
}

std::vector<std::string> & Location::cgi_ext() {
    return _cgiExt;
}

void Location::set_cgi_ext(std::vector<std::string> cgi_ext) {
    _cgiExt = cgi_ext;
}

void Location::setPath(std::string path) {
    this->_path = path;
}

std::string Location::getPath() {
    return this->_path;
}

void Location::setRoot(std::string &root) {
    this->_root = root;
}

void Location::setIndex(std::string &index) {
    this->_index = index;
}

void Location::setAutoIndex(bool autoindex) {
    this->_autoindex = autoindex;
}

void Location::setMethods(std::vector<std::string> methods) {
    std::vector<std::string> newMethods;

    this->_methods.clear();
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++) {
        newMethods.push_back(*it);
    }
    this->_methods = newMethods;
}

std::vector<std::string> Location::getMethods() {
    return this->_methods;
}

void Location::setReturn(std::vector<std::string> returnPage) {
    std::vector<std::string> newReturn;

    this->_return.clear();
    for (std::vector<std::string>::iterator it = returnPage.begin(); it != returnPage.end(); it++) {
        newReturn.push_back(*it);
    }
    this->_return = newReturn;
}

std::vector<std::string> Location::getReturn() {
    return _return;
}

void Location::setClientMaxBodySize(std::basic_string<char> clientMaxBodySize) {
    this->_clientMaxBodySize = clientMaxBodySize;
}

bool Location::getIsCgi() const {
    return _isCgi;
}

void Location::setIsCgi(bool isCgi) {
    _isCgi = isCgi;
}

bool Location::isIsRoot() const {
    return _isRoot;
}

void Location::setIsRoot(bool isRoot) {
    _isRoot = isRoot;
}
