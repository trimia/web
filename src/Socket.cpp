#include "../include/Socket.hpp"
Socket::Socket() {
    //TODO maybe try catch block
    this->_fd_sock=INVALID_SOCKET;
    this->_fd_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(this->_fd_sock==INVALID_SOCKET)
    {
        std::cout<<"error in creating socket"<<std::endl;
//        return false;
    }
    // else
        // std::cout<<"socket successfully created"<<std::endl;
//    return true;
}

Socket::Socket(int fdSock) : _fd_sock(fdSock) {}

Socket::~Socket()
{
//    std::cout << "Socket : Destructor Called" << std::endl;
}

Socket::Socket(Socket const &obj)
{
//    std::cout << "Copy Constructor Called" << std::endl;
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


        //	this->attributes = obj.attributes;
        //	...
    }
    return (*this);
}


/*
 * setsocketoption:
 * choose what option socket hav to do: keepalive etc... see the link in TODO for differnt option
 * optname value SO_RCVTIMEO =20 for client or SO_REUSEADDR = 2 for server
 */
bool Socket::_setSocketOption(int optName) {
    int optVal = 1;
    //understand if the cast to char is necessary!!
    if (setsockopt(this->_fd_sock, SOL_SOCKET, optName, (char *) &optVal, sizeof(optVal)) ==
        SOCKET_ERROR) {
        std::cout << "cannot set socket option" << GETSOCKETERRNO() << std::endl;
        return false;
    } else
        return true;
}

void Socket::_initializeService(char *ip, uint16_t port)
{
    this->_service.sin_family=AF_INET;
    //inet_addr or htonl
    this->_service.sin_addr.s_addr= inet_addr(ip);
    this->_service.sin_port= htons(port);
    this->_sockSize= sizeof(this->_service);
}

bool Socket::_bindSocket(char * ip,uint16_t port) {
//    sockaddr_in service;
    _initializeService(ip,port);
    if((int)bind(this->_fd_sock,(sockaddr*)&this->_service, sizeof(this->_service)) == SOCKET_ERROR)
    {
        std::cout<<"bind failed"<<GETSOCKETERRNO()<<std::endl;
        return (false);
    }
    else{

        return true;
    }
}

bool Socket::_listenOnSocket() {
    //choose how much connection ????
    if(listen(this->_fd_sock,MAX_N_CONNECTION))
    {
        std::cout<<"error on listen"<<GETSOCKETERRNO()<<std::endl;
        return false;
    }else{
        std::cout<<"listen is ok waiting for connection"<<std::endl;
        return true;
    }
}


bool Socket::createServerSock(int optName, char *ip, uint16_t port)
{
    // std::cout<<std::endl<<MAGENTA<<"***************    IP  ********************"<<ip<<RESET_COLOR<<std::endl;
    if (_setSocketOption(optName))
        return false;
    if (_bindSocket(ip,port))
        return false;
    if (_listenOnSocket())
        return false;
    fcntl(this->_fd_sock,F_SETFL,O_NONBLOCK);
    return true;
}

bool Socket::setClientSock(int optName, char *ip, uint16_t port)
{
    _initializeService(ip,port);
    //TODO think if is necessary change socketopt for client considering time ??
    if(!_setSocketOption(optName))
        return false;
    //TODO F_SETFL o F_GETFL??
    fcntl(this->_fd_sock,F_SETFL,O_NONBLOCK);
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
        std::cout<<"connection to socket failed"<<GETSOCKETERRNO()<<std::endl;
        return false;
    }else
    {
        std::cout<<"client connect can timeStart sending and receiving data"<<std::endl;
        return true;
    }
}

int Socket::sendData(SOCKET connectedSocket, Response msg) {
    int byteCount = (int)send(connectedSocket,(char*)&msg.getContent() , sizeof(msg), 0);
    if(byteCount==SOCKET_ERROR)
    {
        std::cout<<"send error"<<GETSOCKETERRNO()<<std::endl;
        return SOCKET_ERROR;
    }else
    {
        std::cout<<"send "<<byteCount<<" byte"<<std::endl;
        return byteCount;
    }
}


/*
 * getter and setter
 */
int Socket::getFdSock() const {
    return _fd_sock;
}
void Socket::setFdSock(int fdSock) {
    _fd_sock = fdSock;
}

const sockaddr_in &Socket::getService() const {
    return _service;
}

void Socket::setService(const sockaddr_in &service) {
    _service = service;
}

socklen_t Socket::getSockSize() const {
    return _sockSize;
}

void Socket::setSockSize(socklen_t sockSize) {
    _sockSize = sockSize;
}
