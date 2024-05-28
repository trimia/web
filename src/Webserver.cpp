#include "../include/Webserver.hpp"

Webserver::Webserver()
{
//	std::cout << "Webserver : Default Constructor Called" << std::endl;
}

Webserver::Webserver(char * conf)
{
    _initEpoll();
    ConfigBlock     confBlock;
    ConfigParser    confParser(conf, confBlock);
    this->_listOfServer=confParser.parseConfigFile();
    std::cout<<std::boolalpha<<YELLOW<<"islocation: "<<this->_listOfServer[1].is_location()<<RESET_COLOR<<std::endl;

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
 *  - init epoll
 *  - add server to epoll
 *  - main loop
 *  - handle epoll events
 */

bool Webserver::runEpoll()
{
    // if(this->_initEpoll()) {
    //     std::cout<<"Error init epoll"<<std::endl;
    //     return false;
    // }
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
                return true;
            }
        }
        sleep(3);
    } while (eventNumber>=0);
    if(eventNumber<0) {
        std::cout<<RED<<"Error epoll wait"<<RESET_COLOR<<std::endl;
        return false;
    }
        return false;
    return true;
}


bool Webserver::_handleEpollEvents(int eventNumber, epoll_event (&events)[MAX_EVENTS]) {
    // std::cout<<"handling epoll events"<<std::endl;
    for (int i = 0; i < eventNumber; ++i)
    {
        std::cout<<"handling event number: "<<i<<std::endl;
        sType *type =static_cast<sType*>(events[i].data.ptr);
        // std::cout<<"ptr->socketType: "<<type->socketType<<std::endl;
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
        }else if((events[i].events & EPOLLIN) || (events[i].events & EPOLLOUT))
        {
            // Client *client = static_cast<Client *>(events[i].data.ptr);
            // std::cout<<MAGENTA<<"handling epoll events"<<std::endl;
            // std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
            // std::cout<<"client socket fd: "<<client->_clientSock->getFdSock()<<std::endl;
            // std::cout<<"client socket service: "<<ntohs(client->_clientSock->getService().sin_port)<<std::endl;
            // std::cout<<"client socket service: "<<inet_ntoa(client->_clientSock->getService().sin_addr)<<std::endl;
            // std::cout<<"client socket size: "<<*client->_clientSock->getSockSize()<<std::endl;
            // std::cout << "Service sin family: " << client->_clientSock->getService().sin_family<<RESET_COLOR<< std::endl;
            if(_handleConnection(events[i])) {
                std::cout<<RED<<"Error handling connection"<<RESET_COLOR<<std::endl;
                return false;

            }
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
 */

bool Webserver::_acceptConnection(Server *server) {
    // socklen_t temp=sizeof(server->_server_socket->getService());
    // std::cout<<BLUE<<"accepting connection"<<std::endl;
    // std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
    // std::cout<<"server socket fd: "<<server->_server_socket->getFdSock()<<std::endl;
    // std::cout<<"server socket service: "<<ntohs(server->_server_socket->getService().sin_port)<<std::endl;
    // std::cout<<"server socket service: "<<inet_ntoa(server->_server_socket->getService().sin_addr)<<std::endl;
    // std::cout<<"server socket size: "<<*server->_server_socket->getSockSize()<<std::endl;
    // std::cout << "Service sin family: " << server->_server_socket->getService().sin_family<<RESET_COLOR<< std::endl;
    int clientFd = accept(server->getserver_socket()->getFdSock(),(sockaddr *)&server->getserver_socket()->getService(),server->getserver_socket()->getSockSize());
    std::cout<<CYAN<<"client fd from accept: "<<clientFd<<RESET_COLOR<<std::endl;
    if (clientFd == -1) {
        std::cout <<RED<< "Error accepting connection: " << RESET_COLOR<<std::endl;
        return false;
    }
    // Client *client;
    // client = new Client();
    Client client;
    // client.getClientSock()->setFdSock(clientFd);
    client.initSocket((char *)server->getIp().c_str(),server->getPort(),CLIENT_SOCK,clientFd);
    // client.set_allowmethods(server->allowmethods());
    std::cout<<BLUE<<"accepting connection"<<std::endl;
    std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
    std::cout<<"client socket fd: "<<client._clientSock->getFdSock()<<std::endl;
    std::cout<<"client socket service: "<<ntohs(client._clientSock->getService().sin_port)<<std::endl;
    std::cout<<"client socket service: "<<inet_ntoa(client._clientSock->getService().sin_addr)<<std::endl;
    std::cout<<"client socket size: "<<*client._clientSock->getSockSize()<<std::endl;
    std::cout << "Service sin family: " << client._clientSock->getService().sin_family<<RESET_COLOR<< std::endl;
    // std::cout<<MAGENTA<<client.getClientSock()->getFdSock() << "  accepted connection" << std::endl;
    // std::cout<<MAGENTA<<"setting client sock"<<RESET_COLOR<<std::endl;
    client._event.events=EPOLLIN | EPOLLOUT;
    client.socketType=CLIENT_SOCK;
    // std::cout<<YELLOW<<"socket type"<<client.socketType<<RESET_COLOR<<std::endl;
    // client.getClientSock()->setClientSock(SO_REUSEADDR,(char *)server->getIp().c_str(),server->getPort());
    // this->addClientToList(client);
    this->_listOfClient.push_back(client);
    client._event.data.ptr=&this->_listOfClient.back();
    if(epoll_ctl(this->_epollFd,EPOLL_CTL_ADD, client._clientSock->getFdSock(), &client._event)<0)
    {
        std::cout<<RED<<"Error adding connection to epoll "<<RESET_COLOR<<std::endl;
        return false;

    }
    std::cout<<GREEN<<"client sock added to epoll instance"<<RESET_COLOR<<std::endl;
    return true;
}

bool Webserver::_handleConnection(epoll_event &event) {
    std::cout<<BLUE<<"handling connection"<<std::endl;
    Client& client= *reinterpret_cast<Client*>(event.data.ptr);
    // client._request=new Request();
    // client.set_request(new Request());
    client.initRequest();
    // Client * client = static_cast<Client *>(event.data.ptr);
    std::cout<<"epollfd: "<<this->_epollFd<<std::endl;
    std::cout<<"client socket fd: "<<client._clientSock->getFdSock()<<std::endl;
    std::cout<<"client socket service: "<<ntohs(client._clientSock->getService().sin_port)<<std::endl;
    std::cout<<"client socket service: "<<inet_ntoa(client._clientSock->getService().sin_addr)<<std::endl;
    std::cout<<"client socket size: "<<*client._clientSock->getSockSize()<<std::endl;
    std::cout << "Service sin family: " << client._clientSock->getService().sin_family<<RESET_COLOR<< std::endl;
    if(client._request->time_start() == 0)
            client._request->set_time_start(std::time(NULL));
    std::time_t currentTime = std::time(NULL);
    double elapsedTime = std::difftime(currentTime, client._request->time_start());
    // Request request;
    // request.receiveData(&client);
    //TODO remember to free request
    // Request *req = new Request();
    // client._request(req);
    // client.initRequest();
    client._request->receiveData(&client);
    // client.request()->receiveData(&client);
    std::cout<<MAGENTA<<"handling connection"<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"body size :"<<client.request()->body_size()<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"header size :"<<client.request()->header_size()<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"time start :"<<client.request()->time_start()<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"error :"<<client.request()->error()<<RESET_COLOR<<std::endl;
    // std::cout<<MAGENTA<<"cgi :"<<client.request()->cgi()<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"ended :"<<client.request()->ended()<<RESET_COLOR<<std::endl;
    std::cout<<MAGENTA<<"method :"<<client.request()->method()<<RESET_COLOR<<std::endl;
    // std::cout<<MAGENTA<<std::endl<<"http message :"<<std::endl<<client.request()->http_message()<<RESET_COLOR<<std::endl<<std::endl;
    // std::cout<<MAGENTA<<"request headers :"<<client.request()->request_headers()["Host"]<<RESET_COLOR<<std::endl;
    // std::cout<<MAGENTA<<"request headers :"<<client.request()->request_headers()["User-Agent"]<<RESET_COLOR<<std::endl;

    std::cout<<GREEN<<"request received  method: "<<client._request->method()<<RESET_COLOR<<std::endl;
    std::cout<<GREEN<<"request received  ended: "<<client._request->ended()<<RESET_COLOR<<std::endl;
    //understand how to initialize request and what is necessary to read from fd and work on response

    /*
     * TODO working progress understandig if this workflow is good
     *    <Request request(currentTime,elapsedTime);
     *    request.receiveData();
     *    request.parseRequest();
     */

    if (elapsedTime>=15)
    {
        std::cout<<RED<<"Connection timeout"<<RESET_COLOR<<std::endl;
        this->_closeConnection(&client);
        return false;
    }
    else if(client._request->ended())
    {
        std::cout<<GREEN<<"Request ended"<<RESET_COLOR<<std::endl;
        //TODO body? send response
        //pathtofile probably come from client headher or location i've to understand
        std::string pathtofile="";
        client.initResponse();
        client.response()->setResponseForMethod(&client);
        // client._response.sendData(client,readFromFile(pathtofile));

        this->_closeConnection(&client);
        return true;
    }
    else if (client._request->cgi())
    {
        // client.request()
        // client.response()
        //TODO handle cgi
    }
    else if (client._request->error())
    {
        std::cout<<RED<<"Error in request"<<RESET_COLOR<<std::endl;
        this->_closeConnection(&client);
        return false;
    }


//prototype
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

    if(epoll_ctl(this->_epollFd,EPOLL_CTL_DEL,client->getClientSock()->getFdSock(),&client->_event))
        return true;
    if(close(client->getClientSock()->getFdSock()))
        return true;
    return false;
}


std::string Webserver::readFromFile(std::string path) {
    int	body_fd = open(path.c_str(), O_RDONLY);
    struct stat file;
    stat(path.c_str(),&file);

    if (body_fd > 0 && file.st_size > 0)
    {
        char			body[file.st_size];
        memset(body, 0, file.st_size);

        int	size = read(body_fd, body, file.st_size);
        switch (size){
            case -1:
                std::cout<< "read Error"<<std::endl;
            break ;
            case 0:
                std::cout<< "Body size 0"<<std::endl;
            break ;
            default:
                std::cout<< "Body read -> " << size << " Byte"<<std::endl;
        }
        if(body_fd != -1)
            close(body_fd);
        return body;
    }
    return 0;
}

void Webserver::addClientToList(Client client) {

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
