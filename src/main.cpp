//#include "Client.hpp"
#include "Server.hpp"
#include "Webserver.hpp"
#include "config.hpp"
#include "Location.hpp"
#include "include.h"
Webserver *ws_ptr;
void	sigint_handler(int sig)
{
    std::cout << RED << "# SIGINT " << sig << " Called #" << RESET_COLOR << std::endl;
    ws_ptr->~Webserver();
    exit(0);
}

int tryToStart(std::vector<config> listOfConf)
{
    for (const auto &item: listOfConf)
    {

    }
    Webserver webserver;
    ws_ptr=&webserver;
    std::vector<Server> listOfServer;
    std::signal(SIGINT, sigint_handler);


    if()
    {
        return 1;
    }
    return 0;
}
int main(int argc, char** argv)
{
    (void ) argc;
    (void ) argv;
    std::cout<<"bella"<<std::endl;

    /*
     * check arg error
     * check config file ?
     * call parser to take info from config file and create server object to fullfill --> std::vector<Server> listOfServer
     * call setup function to  initialize everything
     * call run (function with main loop)---
     * */
    std::map<short,std::string> err;
    err[404]="not found";
    err[104]="magari";
    err[404]="not found";
    std::vector<short> method(4);
    std::vector<std::string> cgipath;
    cgipath[0]="cgipath";
    std::vector<std::string> cgiext;
    cgiext[0]="cgiext";
    //the idea is the every position of the vector is bound to the enum method and if there is 1 is allowed vice versa not allowed
    for(int i =0;i<4;i++)
        method[i]=1;
    Location location("path","root",true,"index",method ,"return","alias",cgipath,cgiext,8);
    std::vector<Location> locationVector;
    locationVector.push_back(location);
    char * ip=(char*)"127.0.0.1";
    std::vector<config>listOfConfig;
    config pippo(htons(8080), ip, (std::string &) "prova", (std::string &) "", (std::string &) "", 1000, true, err,locationVector);
    listOfConfig.push_back(pippo);
    tryToStart(listOfConfig);
    //example
//    TODO maybe this function could be implemented in Webserver class
//    Server server1;
//    server1.setupServer();
//    listOfServer.push_back(server1);
    //end
//
//    Server webserver;
//
//    webserver.setup(listOfServer);
//    webserver.run(listOfServer);



	return (0);
}
