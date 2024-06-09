#include "../include/Webserver.hpp"

Webserver::Webserver()
{
//	std::cout << "Webserver : Default Constructor Called" << std::endl;
}

Webserver::Webserver(char * conf)
{
    _initEpoll();
    ConfigParser    confParser(conf);
    this->_listOfServer=confParser.parseConfigFile();
    std::cout<<std::boolalpha<<YELLOW<<"islocation: "<<this->_listOfServer[1].is_location()<<RESET_COLOR<<std::endl;
    std::cout<<std::boolalpha<<YELLOW<<"location number: "<<this->_listOfServer[1].location_number()<<RESET_COLOR<<std::endl;

    // for (auto x : _listOfServer) {
    //     printf("%sSNAME: %s PORT: %d IP: %s%s\n", YELLOW, x.getServerName().c_str(), x.getPort(), x.getIp().c_str(), RESET_COLOR);
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
	    this->_epollFd=obj._epollFd;
	    this->_listOfServer=obj._listOfServer;
	    this->_listOfClient=obj._listOfClient;
	    // this->conf=obj.conf;

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

/*  runEpoll:
 *  - add server to epoll
 *  - main loop
 */

bool Webserver::runEpoll()
{

    if(!this->_addServerToEpoll()) {
        std::cout<<"Error adding server to epoll"<<std::endl;
        return false;
    }
    if(this->_mainLoop()) {
        std::cout<<"Error main loop"<<std::endl;
        return false;
    }
    return true;

    // understand if is necessary to allocate event
    //    auto epoll_events = (struct epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
}

bool Webserver::_addServerToEpoll() {
    for (std::vector<Server>::iterator item = this->_listOfServer.begin(); item != this->_listOfServer.end(); ++item)
    {
        item->_event.events=EPOLLIN;
        item->_event.data.ptr=item.base();
        item->socketType=SERVER_SOCK;
        // std::cout<<BLUE<<"adding server to epoll instance"<<std::endl;
        // std::cout<<"socket type 1 for server 2 for client ---->   "<<item->socketType<<std::endl;
        // std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
        // std::cout<<"server socket fd: "<<item->_server_socket->getFdSock()<<RESET_COLOR<<std::endl;

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
        // std::cout<<"main loop"<<std::endl;
        eventNumber= epoll_wait(this->_epollFd,events,MAX_EVENTS,EPOLL_TIMEOUT);
        std::cout<<RED<<"event number: "<<eventNumber<<RESET_COLOR<<std::endl;
        if(eventNumber>0)
        {
            if(!_handleEpollEvents(eventNumber,events)) {
                std::cout<<RED<<"Error handling epoll events"<<RESET_COLOR<<std::endl;
                return false;
            }
        }
        //TODO remove sleep
        sleep(3);
    } while (eventNumber>=0);
    if(eventNumber<0) {
        std::cout<<RED<<"Error epoll wait"<<RESET_COLOR<<std::endl;
        return false;
    }
        // return false;
    return true;
}


bool Webserver::_handleEpollEvents(int &eventNumber, epoll_event (&events)[MAX_EVENTS]) {
    std::cout<<BLUE<<"handling epoll events"<<RESET_COLOR<<std::endl;
    for (int i = 0; i < eventNumber; ++i)
    {
        // if(events[i].data.ptr == NULL)
        //     continue;  // Skip this event if the pointer is null
        std::cout<<"handling event number: "<<i<<std::endl;
        sType *type =static_cast<sType*>(events[i].data.ptr);
        // std::cout<<YELLOW<<"ptr->socketType: "<<type->socketType<<RESET_COLOR<<std::endl;
        if(type->socketType==SERVER_SOCK)
        {
            // std::cout<<MAGENTA<<"server sock"<<RESET_COLOR<<std::endl;
        // Server &server = *reinterpret_cast<Server *>(events[i].data.ptr);
            Server *server = static_cast<Server *>(events[i].data.ptr);
           if(!this->_acceptConnection(server))
           {
               std::cout<<"Error accepting connection"<<std::endl;
                return false;
           }
        }else if(((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT)) && _handleConnection(events[i]) )
        {
            // && !_prova(events[i])
            // _prova(events[i]);
                // std::cout<<RED<<"Error handling connection"<<RESET_COLOR<<std::endl;
                return true;
        }
    }
    return true;
}

bool Webserver::_acceptConnection(Server *server) {
    // std::cout<<BLUE<<"accepting connection"<<RESET_COLOR<<std::endl;

    int clientFd = accept(server->getserver_socket()->getFdSock(),(sockaddr *)&server->getserver_socket()->getService(),server->getserver_socket()->getSockSize());
    std::cout<<CYAN<<"client fd from accept: "<<clientFd<<RESET_COLOR<<std::endl;
    if (clientFd == -1) {
        std::cout <<RED<< "Error accepting connection: " << RESET_COLOR<<std::endl;
        return false;
    }else
        std::cout<<GREEN<<"connection accepted"<<RESET_COLOR<<std::endl;
    Client *client= new Client();
    client->initClient(server,clientFd);
    std::cout<<BLUE<<"socket type: "<<client->socketType<<RESET_COLOR<<std::endl;

    // std::cout<<BLUE<<"accepting connection"<<std::endl;
    // std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
    // std::cout<<"client socket fd: "<<client->_clientSock->getFdSock()<<std::endl;
    // std::cout<<"client socket service: "<<ntohs(client->_clientSock->getService().sin_port)<<std::endl;
    // std::cout<<"client socket service: "<<inet_ntoa(client->_clientSock->getService().sin_addr)<<std::endl;
    // std::cout<<"client socket size: "<<*client->_clientSock->getSockSize()<<std::endl;
    // std::cout << "Service sin family: " << client->_clientSock->getService().sin_family<<RESET_COLOR<< std::endl;

    this->_listOfClient.push_back(client);
    client->_event.data.ptr=client;
    std::cout<<BLUE<<"socket type: "<<this->_listOfClient.back()->socketType<<RESET_COLOR<<std::endl;
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
    std::cout<<MAGENTA<<"handling connection"<<RESET_COLOR<<std::endl;
    //TODO choose wich is better
    // Client * client = static_cast<Client *>(event.data.ptr);
    // bool bull=true;
    sType *type =static_cast<sType*>(event.data.ptr);
    // std::cout<<BLUE<<"ptr->socketType: "<<type->socketType<<RESET_COLOR<<std::endl;

    if(type->socketType==SERVER_SOCK)
    {
        std::cout<<MAGENTA<<"server sock"<<RESET_COLOR<<std::endl;
        return false;
    }
    Client& client= *reinterpret_cast<Client*>(event.data.ptr);
    std::cout<<BLUE<<"socket type: "<<client.socketType<<RESET_COLOR<<std::endl;
    client.initRequest();
    client.initResponse();
    client.initLocation();

    // std::cout<<BLUE<<"handling connection"<<std::endl;
    // std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
    // std::cout<<"client socket fd: "<<client._clientSock->getFdSock()<<std::endl;
    // std::cout<<"client socket service: "<<ntohs(client._clientSock->getService().sin_port)<<std::endl;
    // std::cout<<"client socket service: "<<inet_ntoa(client._clientSock->getService().sin_addr)<<std::endl;
    // std::cout<<"client socket size: "<<*client._clientSock->getSockSize()<<std::endl;
    // std::cout << "Service sin family: " << client._clientSock->getService().sin_family<<RESET_COLOR<< std::endl;

    std::time_t currentTime = std::time(NULL);
    double elapsedTime = std::difftime(currentTime, client.request()->time_start());
    client.request()->receiveData(&client);
    //TODO check su url per cgi
    Cgi Cgi(client.request());
    if(client.response()->status_code()!=1)
        client.response()->setResponseForMethod(&client);
    if (client.request()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in request 1111"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
        // return false;
    }
    if (client.response()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in response"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
        // return false;
    }
    //TODO handle keepalive
    if(client.connection()=="keep-alive" && elapsedTime>=20)
    {
        std::cout<<RED<<"Connection keep-alive timeout"<<RESET_COLOR<<std::endl;
        client.request()->set_connection("close");
        // client.request()->set_complete(true);
        _closeConnection(event);
    }
    if (elapsedTime>=15 && client.request()->connection().compare("close"))
    {
        std::cout<<RED<<"Connection timeout"<<RESET_COLOR<<std::endl;
        client.response()->set_status_code(408);
        // this->_closeConnection(event);
        // return false;
    }
    if(client.request()->connection()=="close" && client.response()->complete())
    {
        std::cout<<GREEN<<"Request ended 111111"<<RESET_COLOR<<std::endl;
        std::cout<<GREEN<<"complete: "<<client.response()->complete()<<RESET_COLOR<<std::endl;
        _closeConnection(event);
        return true;
        // return true;
    }
    if(client.response()->status_code()==0) {
        client.response()->set_status_code(501);
        client.response()->set_error(true);
    }
//    std::cout<<std::boolalpha<<"error: "<<client.response()->error()<<"error: "<<client.request()->error()<<std::endl;
    if(client.response()->error()||client.request()->error()) {
        std::cout<<RED<<"Error in response or request"<<RESET_COLOR<<std::endl;
        client.response()->buildHttpResponseHeader(client.request()->http_version(),StatusString(client.response()->status_code()),
            getMimeType("txt"),0);
        // bull=false;
        // return false;
    }
    std::cout<<std::boolalpha<<"request ended: "<<client.request()->ended()<<" request error: "<<client.request()->error()<<std::endl;
//    if(!client.request()->ended() && !client.request()->error()) {
////        std::cout<<RED<<"Error in request or response"<<RESET_COLOR<<std::endl;
//        return true;
//
//    }
    client.response()->sendData(&client);
    // if(client.response()->ready_to_send()) {
    //     client.response()->sendData(client);
    //     // return false;
    //     // return bull;
    // }
    // std::cout<<std::boolalpha<<"complete: "<<client.response()->complete()<<"connectio=====> "<<client.request()->connection()<<std::endl;
    printCharsAndSpecialChars(client.request()->connection());
    if(client.request()->connection()=="keep-alive"&& client.response()->complete()) {
        std::cout<<GREEN<<"Connection keep-alive"<<RESET_COLOR<<std::endl;
        delete client.response();
        delete client.request();
        client._request=NULL;
        client._event.data.ptr=NULL;
        client._event.events = EPOLLIN;
        client.socketType=CLIENT_SOCK;
        client._event.data.ptr = &client;
        if(epoll_ctl(this->_epollFd, EPOLL_CTL_MOD, client._clientSock->getFdSock(), &client._event) < 0) {
            // Handle error
        }
        // client.socketType=CLIENT_SOCK;
        return true;
    }
    std::cout<<std::boolalpha<<"complete: "<<client.response()->complete()<<" error: "<<client.request()->error()<<std::endl;
    if(client.request()->error() && client.response()->complete())
    {
        std::cout<<RED<<"Error in request 2222"<<RESET_COLOR<<std::endl;
        this->_closeConnection(event);
        return true;
        // return false;
    }
    std::cout<<" connection: "<<client.request()->connection()<<std::boolalpha<<" complete: "<<client.response()->complete()<<std::endl;
    if(client.request()->connection()=="close" && client.response()->complete())
    {
        std::cout<<GREEN<<"Request ended 2222"<<RESET_COLOR<<std::endl;
        std::cout<<GREEN<<"complete: "<<client.response()->complete()<<RESET_COLOR<<std::endl;
        _closeConnection(event);
        return true;
        // return true;
    }
    // if (client.request()->error() && client.response()->complete())
    // {
    //     std::cout<<RED<<"Error in request 1111"<<RESET_COLOR<<std::endl;
    //     this->_closeConnection(event);
    //     return false;
    //     // return false;
    // }
    //TODO understand if is necessary:
    // _closeConnection(event);
    // return _prova( &event);
    return false;
}

bool Webserver::_closeConnection(epoll_event &event) {
//     Client& client= *reinterpret_cast<Client*>(event.data.ptr);
    Client* client = static_cast<Client*>(event.data.ptr);
//TODO check what is necessary free delete from list/vector etc...
    if(client->request())
        delete client->request();
    if(client->response())
        delete client->response();
    std::cout<<MAGENTA<<"close connection fdSock: "<<client->getClientSock()->getFdSock()<<RESET_COLOR<<std::endl;
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_DEL,client->getClientSock()->getFdSock(),&client->_event)==-1) {
        std::cout<<RED<<"Error: client sock not deleted from epoll instance"<<RESET_COLOR<<std::endl;
        return false;
    }
    if(close(client->getClientSock()->getFdSock())==-1)
        return false;
//     _listOfClient.remove_if([&client](const Client& c) { return &c == client; });
    _listOfClient.remove_if([client](const Client* c) { return c == client; });
    // delete client;
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
