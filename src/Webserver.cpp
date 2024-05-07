#include "Webserver.hpp"

Webserver::Webserver()
{
	std::cout << "Webserver : Default Constructor Called" << std::endl;
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
    for (Server &item: this->_listOfServer)
    {
        if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item._server_socket->getFdSock(), &item._event)<1)
            return false;
        item._event.events=EPOLLIN;
        item._event.data.ptr=&item;
        item.socketType=SERVER_SOCK;
//        item.setType(SERVER_SOCK);

    }
    std::cout<<"all server sock added to epoll instance"<<std::endl;
    return true;
}

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
        else if(events[i].events && EPOLLIN | events[i].events && EPOLLOUT)
            _handleConnection(events,eventNumber);

    }
    return true;
}
/*
 * handleConnection:
 * check what to do with connection and
 * call read()? or receiveData() write() or sendData() -see the snippet code in the function-,request, response and parser
 * TODO study and complete _handleconnection understand how we want handle response request etc..
 */
bool Webserver::_handleConnection(epoll_event (&events)[MAX_EVENTS],int i) {

//    events[i].data.ptr



//
//    if (EPOLLIN)
//    {
//
//    }


    int str_len;
    void *buf;
    buf=(void *)"";
    str_len = (int)read(events[i].data.fd, buf, BUFFER_SIZE);
    if (str_len == 0) // close request!
    {
        epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
        close(events[i].data.fd);
        //printf("closed client: %d \n", epoll_events[i].data.fd);
    }
    else
    {
        write(events[i].data.fd, buf, str_len);   // echo!
    }

//    client.peaksize = recv(client.connection_socket.sock_fd, buffer, MAX_HEADER_SIZE, MSG_PEEK);

    return false;
}

void Webserver::run_epoll() {

    this->_initEpoll();
    this->_addServerToEpoll();
    this->_mainLoop();

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
