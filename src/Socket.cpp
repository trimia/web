#include "../include/Socket.hpp"

Socket::Socket() : _fd_sock(INVALID_SOCKET), _sockSize(0) {
    memset(&_service, 0, sizeof(_service));
}

Socket::Socket(int fdSock) : _fd_sock(fdSock) {}

Socket::~Socket()
{
    // if (_fd_sock != INVALID_SOCKET)
    // {
    //     close(_fd_sock);
    // }
//    std::cout << "Socket : Destructor Called" << std::endl;
}

Socket::Socket(Socket const &obj)
{
//    std::cout << "Socket : Copy Constructor Called" << std::endl;
    if (this != &obj)
        *this = obj;
}

Socket	&Socket::operator= (const Socket &obj)
{
//    std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj)
    {
        this->_fd_sock=obj._fd_sock;
        this->_service=obj._service;
        this->_sockSize=obj._sockSize;
    }
    return (*this);
}
bool Socket::createServerSock(int optName, char *ip, uint16_t port,int type)
{
    if(!_initializeService(ip,port,type))
        return false;
    if (!_setSocketOption(optName)) {
        close(this->_fd_sock);
        return false;
    }
    if (!_bindSocket())
        return false;
    if (!_listenOnSocket())
        return false;
    fcntl(this->_fd_sock,F_SETFL,O_NONBLOCK);
    return true;
}

bool Socket::_initializeService(char *ip, uint16_t port,int type)
{
    this->_service.sin_family=AF_INET;
    this->_service.sin_port= htons(port);
    if (ip)
    this->_service.sin_addr.s_addr= inet_addr(ip);
    this->_sockSize= (socklen_t)sizeof(this->_service);
    if(type==SERVER_SOCK)
    {
        this->_fd_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(this->_fd_sock==INVALID_SOCKET)
        {
            std::cout<<RED<<"Error in creating socket"<<RESET_COLOR<<std::endl;
            return false;
        }
    }
    std::cout<<GREEN<<"socket successfully created"<<RESET_COLOR<<std::endl;
    return true;
}

/*
 * setsocketoption:
 * choose what option socket hav to do: keepalive etc... see the link in TODO for differnt option
 * optname value SO_RCVTIMEO =20 for client or SO_REUSEADDR = 2 for server
 */

bool Socket::_setSocketOption(int optName) {
    int optVal = 1;
    if(optName==SO_REUSEADDR)
    {
        if (setsockopt(this->_fd_sock, SOL_SOCKET, optName, (char *) &optVal, sizeof(optVal)) ==
            SOCKET_ERROR) {
            std::cout<<RED << "Error cannot set socket option" << RESET_COLOR<<std::endl;
            return false;
        }
    }
    else
    {
            struct timeval tv;
            tv.tv_sec = 5;
            tv.tv_usec = 0;
        if (setsockopt(this->_fd_sock, SOL_SOCKET, optName, (char *) &tv, sizeof(tv)) ==
            SOCKET_ERROR) {
            std::cout<<RED << "Error cannot set socket option" << RESET_COLOR<<std::endl;
            return false;
        }
    }
    std::cout<<GREEN<<"socket option set"<<RESET_COLOR<<std::endl;
    return true;

}


bool Socket::_bindSocket() {
    if(bind(this->_fd_sock,(sockaddr*)&this->_service, sizeof(this->_service)) == SOCKET_ERROR)
    {
        std::cout<<RED<<"Error bind failed"<<RESET_COLOR<<std::endl;
        return (false);
    }
    std::cout<<GREEN<<"bind is ok"<<RESET_COLOR<<std::endl;
    return true;
}

bool Socket::_listenOnSocket() {
    if(listen(this->_fd_sock,SOMAXCONN)==SOCKET_ERROR)
    {
        std::cout<<RED<<"Error on listen"<<RESET_COLOR<<std::endl;
        return false;
    }
    std::cout<<GREEN<<"listen is ok waiting for connection"<<RESET_COLOR<<std::endl;
    return true;
}

bool Socket::setClientSock(int optName, char *ip, uint16_t port,int type)
{
    _initializeService(ip,port,type);
    if(!_setSocketOption(optName))
        return false;
    int flags = fcntl(this->_fd_sock, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(this->_fd_sock, F_SETFL, flags);
    return true;
}

/*
 *
 */

int Socket::getFdSock() const {
    return _fd_sock;
}

void Socket::setFdSock(int fd_sock) {
    _fd_sock = fd_sock;
}

sockaddr_in &Socket::getService() {
    return _service;
}

void Socket::setService(const sockaddr_in &service) {
    _service = service;
}

socklen_t *Socket::getSockSize(){
    return &_sockSize;
}

void Socket::setSockSize(socklen_t sockSize) {
    _sockSize = sockSize;
}
