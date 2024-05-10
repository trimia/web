#include "../include/Webserver.hpp"

Webserver::Webserver()
{
	std::cout << "Webserver : Default Constructor Called" << std::endl;
}

Webserver::Webserver(std::string conf)
{
    ConfigBlock     confBlock;
    ConfigParser    confParser(conf, confBlock);
    this->_listOfServer=confParser.parseConfigFile();

}

Webserver::~Webserver()
{
	std::cout << "Webserver : Destructor Called" << std::endl;
}

Webserver::Webserver(Webserver const &obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Webserver	&Webserver::operator= (const Webserver &obj)
{
	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}


bool Webserver::_initEpoll() {
    this->_epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (this->_epollFd<0)
        return true;
    std::cout<<"epoll created"<<std::endl;
    return false;
}

bool Webserver::_addServerToEpoll() {

    for (std::vector<Server>::iterator item = this->_listOfServer.begin(); item != this->_listOfServer.end(); ++item)
    {
        if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item->_server_socket.getFdSock(), &item->_event)<1)
//            if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item->getServerSocket()->getFdSock(), *item->getEvent())<1)
            return false;
        item->_event.events=EPOLLIN;
        item->_event.data.ptr=&item;
        item->socketType=SERVER_SOCK;
//        item.setType(SERVER_SOCK);

    }
    std::cout<<"all server sock added to epoll instance"<<std::endl;
    return true;
}
/*
 * It was causing problems during compilation
 * has been commented for testing purpose
 */
bool Webserver::_mainLoop() {
    int eventNumber;
    epoll_event events[MAX_EVENTS];
    do{
        eventNumber= epoll_wait(this->_epollFd,events,MAX_EVENTS,EPOLL_TIMEOUT);
        if(eventNumber>0)
            _handleEpollEvents(eventNumber,events);

    } while (eventNumber<=0);
    return false;
}
bool Webserver::_handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]) {
    for (int i = 0; i < eventNumber; ++i)
    {
        sType 	*ptr = static_cast<sType*>(events[i].data.ptr);
        if(ptr->socketType==SERVER_SOCK){
            Server *server = static_cast<Server *>(events[i].data.ptr);
           if(server->_server_socket->acceptConnection(server, this->_epollFd, this))
                return false;
        }
        else if(((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT)) &&
                _handleConnection(events[i]))
//            _handleConnection(events,eventNumber))
        {
            std::cout<<"error handling connection"<<std::endl;
            return false;
        }

    }
    return true;
}
/*
 * handleConnection:
 * check what to do with connection and
 * call read()? or receiveData() write() or sendData() -see the snippet code in the function-,request, response and parser
 * TODO study and complete _handleconnection understand how we want handle response request etc..
 *
 * TODO remy garcia's compilation says :
 * The error message indicates a potential buffer overflow issue in the line: /////////////////
 *  str_len = (int)read(event.data.fd, buf, BUFFER_SIZE);
 */

bool Webserver::_acceptConnection(Server *server) {
//    SOCKET acceptSocket;
//    acceptSocket= accept(server._fd,server._server_socket._service, sizeof(server._server_socket._service));
//    if(acceptSocket==INVALID_SOCKET)
    Client client;
//    Server *ptr = static_cast<Server*>(event.data.ptr);
//    Socket *ptr1 = static_cast<Socket*>(&ptr->_server_socket);
//    Socket *ptr = static_cast<Socket*>(server->_server_socket.getFdSock());
//    &ptr._service;
//    socklen_t &tmp = static_cast<socklen_t&>(server->_server_socket.getSockSize());

socklen_t temp=sizeof(server->_server_socket.getService());
//temp=server._server_socket.getSockSize();
//TODO fix the problem with socklen_t and  the getter ;(
//    if((client._clientSock._fd_sock = accept(server->_server_socket->_fd_sock, (sockaddr *) &server->_server_socket->_service,&server->_server_socket->_sockSize)) == INVALID_SOCKET)
        if(( client.setClientFdSock(accept(server->_server_socket.getFdSock(),(sockaddr *) &server->_server_socket.getService(),&temp)) ) == INVALID_SOCKET) {
            std::cout << "accepted failed" << GETSOCKETERRNO() << std::endl;
            return false;
        }
    client._event.events=EPOLLIN | EPOLLOUT;
    client._event.data.ptr=&client;
    client.socketType=CLIENT_SOCK;
    this->addClientToList(client);
//    webserver->addClientToList(client);
    fcntl(client._clientSock.getFdSock(),F_SETFL,O_NONBLOCK);
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, client._clientSock.getFdSock(), &client._event)<1)
        return false;
    std::cout<<"client sock added to epoll instance"<<std::endl;
    return true;
}

bool Webserver::_handleConnection(epoll_event &event) {

//    events[i].data.ptr
//    sType 	*ptr = static_cast<sType*>(events[i].data.ptr);
    sConnection& ptr= *reinterpret_cast<sConnection*>(event.data.ptr);
    Request request;

    //understand how to initialize request and what is necessary to read from fd and work on response


    if(ptr.timeStart == 0)
        ptr.timeStart=std::time(NULL);
    std::time_t currentTime = std::time(NULL);
    double elapsedTime = std::difftime(currentTime, ptr.timeStart);
    if (elapsedTime>=15)
    {

        return false;
    }
//
//    int str_len;
//    void *buf;
//    buf=(void *)"";
//    str_len = (int)read(event.data.fd, buf, BUFFER_SIZE);
//    if (str_len == 0) // close request!
//    {
//        epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, event.data.fd, &event);
//        close(event.data.fd);
//        //printf("closed client: %d \n", epoll_events[i].data.fd);
//        return true;
//    }
//    else
//    {
//        write(event.data.fd, buf, str_len);   // echo!
//    }

//    client.peaksize = recv(client.connection_socket.sock_fd, buffer, MAX_HEADER_SIZE, MSG_PEEK);

    return false;
}


bool Webserver::_closeConnection(epoll_event &event) {
    (void)event;
    //TODO handle keepalive

//    if(epoll_ctl(this->_epollFd,EPOLL_CTL_DEL,))
//        return true;
//    if(close(fd))
//        return true;


    return false;
}

/*
 * Trimia this is a fake and modified runEpoll
 * by remy garcia
 * just to let the shit run!
 * yours is the next one (untouched)
 * */
//bool Webserver::runEpoll()
//{
//    bool a,b = false;//,c= false;
//    a=this->_initEpoll();
//    b=this->_addServerToEpoll();
////    c=this->_mainLoop();
//    if(a == false || b == false)// || c == false)
//        return false;
//    return true;
//
//// understand if is necessary to allocate event
////    auto epoll_events = (struct epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
////    struct epoll_event event;
//}

bool Webserver::runEpoll()
{
    bool a,b,c= false;
    a=this->_initEpoll();
    b=this->_addServerToEpoll();
    c=this->_mainLoop();
    if(a == false || b == false || c == false)
        return false;
    return true;

// understand if is necessary to allocate event
//    auto epoll_events = (struct epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
//    struct epoll_event event;
}

void Webserver::addClientToList(Client client) {

    this->_listOfClient.push_back(client);

}

/*
 *
 *                  getter and setter
 *
 */
const std::vector<Server> &Webserver::getListOfServer() const {
    return _listOfServer;
}

void Webserver::setListOfServer(const std::vector<Server> &listOfServer) {
    _listOfServer = listOfServer;
}

const std::vector<Client> &Webserver::getListOfClient() const {
    return _listOfClient;
}

void Webserver::setListOfClient(const std::vector<Client> &listOfClient) {
    _listOfClient = listOfClient;
}

int Webserver::getEpollFd() const {
    return _epollFd;
}

void Webserver::setEpollFd(int epollFd) {
    _epollFd = epollFd;
}
