#include "Cgi.hpp"

Cgi::Cgi()
{
	std::cout << "Cgi : Default Constructor Called" << std::endl;
}

Cgi::~Cgi()
{
	std::cout << "Cgi : Destructor Called" << std::endl;
}

Cgi::Cgi(Cgi const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Cgi	&Cgi::operator= (const Cgi &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}
