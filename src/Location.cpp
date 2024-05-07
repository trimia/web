#include "Location.hpp"

Location::Location()
{
	std::cout << "Location : Default Constructor Called" << std::endl;
}


Location::Location(const std::string &path, const std::string &root, bool autoindex, const std::string &index,
                   const std::vector<short> &methods, std::string ret, std::string alias, std::vector<std::string> cgiPath,
                   std::vector<std::string> cgiExt,unsigned long  clientMaxBodySize):
                   _path(path), _root(root), _autoindex(autoindex), _index(index),
                   _methods(methods), _return(ret), _alias(alias), _cgiPath(cgiPath),
                   _cgiExt(cgiExt), _clientMaxBodySize(clientMaxBodySize) {}


Location::~Location()
{
	std::cout << "Location : Destructor Called" << std::endl;
}

Location::Location(Location const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Location	&Location::operator= (const Location &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}
