#include "../include/Location.hpp"

Location::Location()
{
//	std::cout << "Location : Default Constructor Called" << std::endl;
}


Location::Location(const std::string &path, const std::string &root, bool autoindex, const std::string &index,
                   std::allocator<std::basic_string<char> > methods, std::allocator<std::basic_string<char> > ret, std::string alias, std::vector<std::string> cgiPath,
                   std::vector<std::string> cgiExt, unsigned long  clientMaxBodySize):
                   _path(path), _root(root), _autoindex(autoindex), _index(index),
                   _methods(methods), _return(ret), _alias(alias), _cgiPath(cgiPath),
                   _cgiExt(cgiExt), _clientMaxBodySize(reinterpret_cast<const char *>(clientMaxBodySize)) {}


Location::~Location()
{
//	std::cout << "Location : Destructor Called" << std::endl;
}

Location::Location(Location const &obj)
{
//	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Location	&Location::operator= (const Location &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj)
    {
        this->_path=obj._path;
        this->_cgiPath=obj._cgiPath;
        this->_root=obj._root;
        this->_index=obj._index;
        this->_clientMaxBodySize=obj._clientMaxBodySize;
        this->_autoindex=obj._autoindex;
        this->setMethods(obj._methods);
//        this->_error_pages=obj._error_pages;
    }
    return (*this);
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
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++) {
        this->_methods.push_back(*it);
    }
}

std::vector<std::string> Location::getMethods() {
    return this->_methods;
}

void Location::setReturn(std::vector<std::string> &returnPage) {
    this->_return = returnPage;
}

void Location::setCgiPath(std::vector<std::string> &cgiPath) {
    this->_cgiPath = cgiPath;
}

void Location::setClientMaxBodySize(std::basic_string<char> clientMaxBodySize) {
    this->_clientMaxBodySize = clientMaxBodySize;
}
