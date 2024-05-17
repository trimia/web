#include "../include/Webserver.hpp"

Webserver::Webserver()
{
//	std::cout << "Webserver : Default Constructor Called" << std::endl;
}

Webserver::Webserver(char * conf)
{
    ConfigBlock     confBlock;
    ConfigParser    confParser(conf, confBlock);
    this->_listOfServer=confParser.parseConfigFile();

    // for (auto x : _listOfServer) {
    //     printf("%sSNAME: %s PORT: %d IP: %s%s\n", YELLOW, x.getServerName().c_str(), x.getPort(), x.getIp(), RESET_COLOR);
    // }

}

Webserver::~Webserver()
{
//	std::cout << "Webserver : Destructor Called" << std::endl;
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
            //            if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item->getServerSocket()->getFdSock(), *item->getEvent())<1)
        if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item->_server_socket.getFdSock(), &item->_event)<1)
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
    int eventNumber=0;
    epoll_event events[MAX_EVENTS];
    do{
        eventNumber= epoll_wait(this->_epollFd,events,MAX_EVENTS,EPOLL_TIMEOUT);
        if(eventNumber>0)
        {
            if(_handleEpollEvents(eventNumber,events))
                return true;
        }

    } while (eventNumber>=0);
    if(eventNumber<0)
        return false;
    return true;
}

bool Webserver::_handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]) {
    for (int i = 0; i < eventNumber; ++i)
    {
        sType 	*ptr = static_cast<sType*>(events[i].data.ptr);
        if(ptr->socketType==SERVER_SOCK)
        {
            Server *server = static_cast<Server *>(events[i].data.ptr);
           if(this->_acceptConnection(server))
                return false;
        }
        else if(((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT)) && _handleConnection(events[i]))
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
    Client client;
    socklen_t temp=sizeof(server->_server_socket.getService());
    if ((client.setClientFdSock(accept(server->_server_socket.getFdSock(),
                                       (sockaddr *) &server->_server_socket.getService(),
                                       &temp))) == INVALID_SOCKET) {
        std::cout <<RED << "accepted failed" <<RESET_COLOR << std::endl;
        return false;
    }
    client._event.events=EPOLLIN | EPOLLOUT;
    client._event.data.ptr=&client;
    client.socketType=CLIENT_SOCK;
    client.getClientSock().setClientSock(SO_REUSEADDR,(char *)server->getIp().c_str(),server->getPort());
    this->addClientToList(client);
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, client._clientSock.getFdSock(), &client._event)<1)
        return false;
    std::cout<<"client sock added to epoll instance"<<std::endl;
    return true;
}

bool Webserver::_handleConnection(epoll_event &event) {

//    events[i].data.ptr
//    sType 	*ptr = static_cast<sType*>(events[i].data.ptr);
    sConnection& ptr= *reinterpret_cast<sConnection*>(event.data.ptr);
    Client * client = static_cast<Client *>(event.data.ptr);
    if(ptr.timeStart == 0)
            ptr.timeStart=std::time(NULL);
    std::time_t currentTime = std::time(NULL);
    double elapsedTime = std::difftime(currentTime, ptr.timeStart);
    //understand how to initialize request and what is necessary to read from fd and work on response

    /*
     * TODO working progress understandig if this workflow is good
     *    <Request request(currentTime,elapsedTime);
     *    request.receiveData();
     *    request.parseRequest();
     */

if (elapsedTime>=15)
    {
        this->_closeConnection(client);
        return false;
    }
    else if (ptr.cgi)
    {
        //TODO handle cgi
    }
    else if(ptr.ended)
    {
        this->_closeConnection(client);

    }
    else if (ptr.error)
    {
        this->_closeConnection(client);
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


bool Webserver::_closeConnection(Client *client) {

    //TODO handle keepalive

    if(epoll_ctl(this->_epollFd,EPOLL_CTL_DEL,client->getClientSock().getFdSock(),&client->_event))
        return true;
    if(close(client->getClientSock().getFdSock()))
        return true;
    return false;
}


bool Webserver::runEpoll()
{
    if(this->_initEpoll())
        return false;
    if(this->_addServerToEpoll())
        return false;
    if(this->_mainLoop())
        return false;
    return true;

// understand if is necessary to allocate event
//    auto epoll_events = (struct epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
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
