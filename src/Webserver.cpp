#include "../include/Webserver.hpp"
//today
Webserver::Webserver()
{
//	std::cout << "Webserver : Default Constructor Called" << std::endl;
}

Webserver::Webserver(char * conf)
{
    _initEpoll();
    ConfigParser    confParser(conf);
    this->_listOfServer=confParser.parseConfigFile();

}

Webserver::~Webserver()
{
//	std::cout << "Webserver : Destructor Called" << std::endl;
}

Webserver::Webserver(Webserver const &obj)
{
//	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Webserver	&Webserver::operator= (const Webserver &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
	if (this != &obj)
	{
	    this->_epollFd=obj._epollFd;
	    this->_listOfServer=obj._listOfServer;
	    this->_listOfClient=obj._listOfClient;
	}
	return (*this);
}


bool Webserver::_initEpoll() {
    this->_epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (this->_epollFd<0)
        return true;
    std::cout<<GREEN<<"epoll created"<<RESET_COLOR<<std::endl;
    return false;
}

/*  runEpoll:
 *  - add server to epoll
 *  - main loop
 */

bool Webserver::runEpoll()
{

    if(!this->_addServerToEpoll()) {
        std::cout<<RED<<"Error adding server to epoll"<<RESET_COLOR<<std::endl;
        return false;
    }
    if(!this->_mainLoop()) {
        std::cout<<RED<<"Error main loop"<<RESET_COLOR<<std::endl;
        return false;
    }
    return true;
}

bool Webserver::_addServerToEpoll() {
    for (std::vector<Server>::iterator item = this->_listOfServer.begin(); item != this->_listOfServer.end(); ++item)
    {
        item->_event.events=EPOLLIN;
        item->_event.data.ptr=item.base();
        item->socketType=SERVER_SOCK;
        if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, item->_server_socket->getFdSock(), &item->_event)<0) {
            std::cout<<RED<< "Error adding connection to epoll instance " << RESET_COLOR << std::endl;
            return false;
        }
    }
    std::cout<<GREEN<<"all server sock added to epoll instance"<<RESET_COLOR<<std::endl;
    return true;
}

bool Webserver::_mainLoop() {
    int eventNumber=0;
    epoll_event events[MAX_EVENTS];
    do{
        eventNumber= epoll_wait(this->_epollFd,events,MAX_EVENTS,EPOLL_TIMEOUT);
        if(eventNumber>0)
        {
            if(!_handleEpollEvents(eventNumber,events)) {
                std::cout<<RED<<"Error handling epoll events"<<RESET_COLOR<<std::endl;
                return false;
            }
        }
    } while (eventNumber>=0);
    if(eventNumber<0) {
        return false;
    }
    return true;
}


bool Webserver::_handleEpollEvents(int &eventNumber, epoll_event (&events)[MAX_EVENTS]) {
    for (int i = 0; i < eventNumber; ++i)
    {
        sType *type =static_cast<sType*>(events[i].data.ptr);
        if(type->socketType==SERVER_SOCK)
        {
            Server *server = static_cast<Server *>(events[i].data.ptr);
           if(!this->_acceptConnection(server))
           {
               std::cout<<"Error accepting connection"<<std::endl;
               return false;
           }
        }else if(((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT)) && _handleConnection(events[i]) )
        {
                return true;
        }
    }
    return true;
}

bool Webserver::_acceptConnection(Server *server) {
    int clientFd = accept(server->getserver_socket()->getFdSock(),(sockaddr *)&server->getserver_socket()->getService(),server->getserver_socket()->getSockSize());
    if (clientFd == -1) {
        std::cout<<RED<<"Error in accepting connection"<<RESET_COLOR<<std::endl;
        return false;
    }else
        std::cout<<GREEN<<"connection accepted"<<RESET_COLOR<<std::endl;
    Client *client= new Client();
    client->initClient(server,clientFd);
    this->_listOfClient.push_back(client);
    client->_event.data.ptr=client;
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, client->_clientSock->getFdSock(), &client->_event)<0)
    {
        std::cout<<RED<<"Error adding client to epoll "<<RESET_COLOR<<std::endl;
        return false;

    }
    std::cout<<GREEN<<"client sock added to epoll instance"<<RESET_COLOR<<std::endl;
    return true;
}

/*
 * handleConnection:
 * check what to do with connection and
 * call read()? or receiveData() write() or sendData() -see the snippet code in the function-,request, response and parser
 * TODO study and complete _handleconnection understand how we want handle response request etc..
 * REMEMBER false keep in handleConnection true return to main loop
 */



bool Webserver::_handleConnection(epoll_event &event) {
    sType *type =static_cast<sType*>(event.data.ptr);
    if(type->socketType==SERVER_SOCK)
    {
        return false;
    }
    Client& client= *reinterpret_cast<Client*>(event.data.ptr);
    if(!client.get_not_complete())
    {
        if(client._event.events & EPOLLIN){
            client.initRequest();
        }
        client.initResponse();
    }
    std::time_t currentTime = std::time(NULL);
    double elapsedTime = std::difftime(currentTime, client.request()->time_start());
    if(client._event.events & EPOLLIN )
        client.request()->receiveData(&client);
    if(client.get_not_complete())
        return true;
    if (client._event.events & EPOLLIN && client.request()->ended()) {
        std::cout<<GREEN<<"Request ended set to epollout"<<RESET_COLOR<<std::endl;
        client._event.events = EPOLLOUT;
        if(epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, client._clientSock->getFdSock(), &client._event) < 0) {
        }
    }
    if(client.response()->status_code()==1) {
        std::cout<<RED<<"501 internal server error"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
    }
     if(client.response()->status_code()==204) {
         _closeConnection(event);
         return true;
     }
    if(client._event.events & EPOLLOUT && client.request()->ended() && !client.request()->error() && client.response()->status_code()!=204) {
        std::cout<<YELLOW<<"handling response"<<RESET_COLOR<<std::endl;
        client.response()->setResponseForMethod(&client);

    }
    if (client.request()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in request"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
    }
    if (client.response()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in response"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
    }
    double keepAliveelapsedTime = std::difftime(currentTime, client.time_start());
    if(client.connection()=="keep-alive" && keepAliveelapsedTime>=20)
    {
        std::cout<<YELLOW<<"Connection keep-alive timeout"<<RESET_COLOR<<std::endl;
        client.request()->set_connection("close");
        _closeConnection(event);
    }
    if (elapsedTime>=15 && client.request()->connection()=="close")
    {
        std::cout<<YELLOW<<"Connection timeout"<<RESET_COLOR<<std::endl;
        client.response()->set_status_code(408);
    }
    if(client.request()->connection()=="close" && client.response()->complete())
    {
        std::cout<<GREEN<<"Request ended"<<RESET_COLOR<<std::endl;
        _closeConnection(event);
        return true;
    }
    if(client._event.events & EPOLLOUT && ((client.response()->error() || client.request()->error()) && client.response()->status_code()!=204)) {
        std::cout<<RED<<"Error in response or request"<<RESET_COLOR<<std::endl;
        if(client.response()->getReturn_())
            client.response()->buildRedirectResponseHeader(client.request()->http_version(),StatusString(client.response()->status_code()),client.response()->getLocation());
    }
    if(!client.request()->ended() && !client.request()->error()) {
        return true;
    }
    if(client._event.events & EPOLLOUT)
        client.response()->sendData(&client);
    if (client._event.events & EPOLLOUT && client.response()->complete()) {
        std::cout<<GREEN<<"Response ended set to epollin"<<RESET_COLOR<<std::endl;
        client._event.events = EPOLLIN;
        if (epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, client._clientSock->getFdSock(), &client._event) < 0) {
            std::cout<<RED<<"Error in epol ctd mod"<<RESET_COLOR<<std::endl;
            client.response()->set_status_code(501);
            client.response()->set_error(true);
        }
    }
    if(client.request()->connection()=="keep-alive"&& client.response()->complete()) {
        if(client.request()){
            delete client.request();
            client.set_request(NULL);
        }
        if(client.response()){
            delete client.response();
            client.set_response(NULL);
        }
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        int flags = fcntl(client.getClientSock()->getFdSock(), F_GETFL, 0);
        flags |= O_NONBLOCK;
        fcntl(client.getClientSock()->getFdSock(), F_SETFL, flags);
        if (setsockopt(client.getClientSock()->getFdSock(), SOL_SOCKET, SO_KEEPALIVE,(char *) &tv, sizeof(tv)) ==SOCKET_ERROR) {
            std::cout<<RED << "cannot set socket option" << RESET_COLOR<<std::endl;
            return false;
        }
        if(epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, client._clientSock->getFdSock(), &client._event) < 0) {
            std::cout<<RED<<"Error in epol ctd mod"<<RESET_COLOR<<std::endl;
            client.response()->set_status_code(501);
            client.response()->set_error(true);
        }
        return true;
    }
    if(client.request()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in request"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
    }
    if(client.request()->connection()=="close" && client.response()->complete())
    {
        std::cout<<GREEN<<"Request ended"<<RESET_COLOR<<std::endl;
        _closeConnection(event);
        return true;
    }
    return false;
}

struct MatchClient {
    Client* target;
    MatchClient(Client* target) : target(target) {}
    bool operator()(const Client* c) const {
        return c == target;
    }
};

bool Webserver::_closeConnection(epoll_event &event) {
    Client* client = static_cast<Client*>(event.data.ptr);
    if(client->request()){
        delete client->request();
        client->set_request(NULL);
    }
    if(client->response()){
        delete client->response();
        client->set_response(NULL);
    }
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_DEL,client->getClientSock()->getFdSock(),&client->_event)==-1) {
        std::cout<<RED<<"Error: client sock not deleted from epoll instance"<<RESET_COLOR<<std::endl;
        return false;
    }
    if(close(client->getClientSock()->getFdSock())==-1){
        std::cout<<RED<<"Error: client sock not closed"<<RESET_COLOR<<std::endl;
        return false;

    }
    _listOfClient.remove_if(MatchClient(client));
    event.data.ptr=NULL;
    client = NULL;
    return true;
}


void Webserver::addClientToList(Client *client) {

    this->_listOfClient.push_back(client);
    std::cout<<"client added to list"<<std::endl;

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

std::list<Client *> Webserver::getListOfClient() const {
    return _listOfClient;
}

void Webserver::setListOfClient(const std::list<Client *> listOfClient) {
    _listOfClient = listOfClient;
}

int Webserver::getEpollFd() const {
    return _epollFd;
}

void Webserver::setEpollFd(int epollFd) {
    _epollFd = epollFd;
}
