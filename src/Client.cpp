#include "Client.hpp"

Client::Client()
{
	std::cout << "Client : Default Constructor Called" << std::endl;
}

Client::Client(int id) : _id(id){}

Client::~Client()
{
	std::cout << "Client : Destructor Called" << std::endl;
}

Client::Client(Client const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Client	&Client::operator= (const Client &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
        this->_id=obj._id;
//        this->_server=obj;
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}
