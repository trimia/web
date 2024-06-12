#include "../include/Socket.hpp"

Socket::Socket() : _fd_sock(INVALID_SOCKET), _sockSize(0) {
    // Initialize _service to zero
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


        //	this->attributes = obj.attributes;
        //	...
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
    // std::cout<<YELLOW<<"ip: "<<ip<<RESET_COLOR<<std::endl;
    this->_service.sin_family=AF_INET;
    this->_service.sin_port= htons(port);
    // inet_pton(AF_INET, ip, &this->_service.sin_addr.s_addr);
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
//    std::cout<<CYAN<<"initialize service"<<std::endl;
//    std::cout<<"server socket fd: "<<this->_fd_sock<<std::endl;
//    std::cout<<"server socket service: "<<ntohs(this->_service.sin_port)<<std::endl;
//    std::cout<<"server socket service: "<<inet_ntoa(this->_service.sin_addr)<<std::endl;
//    std::cout<<"server socket size: "<<this->_sockSize<<std::endl;
//    std::cout << "Service sin family: " << this->_service.sin_family<<RESET_COLOR<< std::endl;
    return true;

    // std::cout<<"port: "<<this->_service.sin_port<<std::endl;
    // inet_pton(AF_INET, ip, &serverAddr.sin_addr.s_addr);
    // inet_pton(AF_INET, ip, &this->_service.sin_addr.s_addr);
}


/*
 * setsocketoption:
 * choose what option socket hav to do: keepalive etc... see the link in TODO for differnt option
 * optname value SO_RCVTIMEO =20 for client or SO_REUSEADDR = 2 for server
 */

bool Socket::_setSocketOption(int optName) {
    int optVal = 1;
    // std::cout<<CYAN<<"set socket option"<<std::endl;
    // std::cout<<"socket fd: "<<this->_fd_sock<<std::endl;
    // std::cout<<"socket service: "<<ntohs(this->_service.sin_port)<<std::endl;
    // std::cout<<"socket service: "<<inet_ntoa(this->_service.sin_addr)<<std::endl;
    // std::cout<<"socket size: "<<this->_sockSize<<std::endl;
    // std::cout << "Service sin family: " << this->_service.sin_family<<RESET_COLOR<< std::endl;
    //understand if the cast to char is necessary!!
    if(optName==SO_REUSEADDR)
    {
        if (setsockopt(this->_fd_sock, SOL_SOCKET, optName, (char *) &optVal, sizeof(optVal)) ==
            SOCKET_ERROR) {
            std::cout<<RED << "cannot set socket option" << RESET_COLOR<<std::endl;
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
            std::cout<<RED << "cannot set socket option" << RESET_COLOR<<std::endl;
            return false;
        }
    }
    std::cout<<GREEN<<"socket option set"<<RESET_COLOR<<std::endl;
    return true;

}


bool Socket::_bindSocket() {
//    sockaddr_in service;
    // std::cout<<CYAN<<"bind socket"<<std::endl;
    // std::cout<<"server socket fd: "<<this->_fd_sock<<std::endl;
    // std::cout<<"server socket service: "<<ntohs(this->_service.sin_port)<<std::endl;
    // std::cout<<"server socket service: "<<inet_ntoa(this->_service.sin_addr)<<std::endl;
    // std::cout<<"server socket size: "<<this->_sockSize<<std::endl;
    // std::cout << "Service sin family: " << this->_service.sin_family<<RESET_COLOR<< std::endl;
    if(bind(this->_fd_sock,(sockaddr*)&this->_service, sizeof(this->_service)) == SOCKET_ERROR)
    {
        std::cout<<RED<<"bind failed"<<RESET_COLOR<<std::endl;
        return (false);
    }
    std::cout<<GREEN<<"bind is ok"<<RESET_COLOR<<std::endl;
    return true;
}

bool Socket::_listenOnSocket() {
    //choose how much connection ????
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
    //TODO think if is necessary change socketopt for client considering time ??
    if(!_setSocketOption(optName))
        return false;
    //TODO F_SETFL o F_GETFL??
    int flags = fcntl(this->_fd_sock, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(this->_fd_sock, F_SETFL, flags);
    return true;
}
/*
 *
 */


bool Socket::connectSocket(SOCKET clientSocket, uint16_t port) {
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&clientService.sin_addr.s_addr);
//    InetPton() is necesary? find something you can use
    clientService.sin_port = htons(port);
    if(connect(clientSocket,(sockaddr*)&clientService, sizeof(clientService))==SOCKET_ERROR)
    {
        std::cout<<"connection to socket failed"<<std::endl;
        return false;
    }else
    {
        std::cout<<"client connect can timeStart sending and receiving data"<<std::endl;
        return true;
    }
}

int Socket::getFdSock() const {
    return _fd_sock;
}

void Socket::setFdSock(int fd_sock) {
    _fd_sock = fd_sock;
}


/*
 * getter and setter
 */
// int Socket::getFdSock() const {
//     return _fd_sock;
// }
// void Socket::setFdSock(int fdSock) {
//     _fd_sock = fdSock;
// }

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
