#include "include/ConfigParser.hpp"
#include "include/ConfigBlock.hpp"
#include "include/Server.hpp"
#include "include/Webserver.hpp"
#include "include/include.h"

Webserver *ws_ptr;

void	sigint_handler(int sig)
{
    std::cout << RED << "# SIGINT " << sig << " Called #" << RESET_COLOR << std::endl;
    ws_ptr->~Webserver();
    exit(0);
}

int tryToStart(Webserver webserver)
{
//necessary only if we have to setup server
//    for (const auto &item: listOfConf)
//    {
//
//    }
    std::signal(SIGINT, sigint_handler);
    if(webserver.runEpoll())
        return 0;
    return 1;
}

int main(int argc, char* argv[]) {
    Webserver webserver;
    ws_ptr=&webserver;


    if (argc != 2) {
        std::cerr << "Error: wrong number of arguments." << std::endl;
        exit(2);
    }
    ConfigBlock     confBlock;
    ConfigParser    confParser(argv[1], confBlock);
    //TODO maybe this take webserver and fill it or return a list of server
//    confParser.parseConfigFile();
    std::cout<<"here we are"<<std::endl;
    std::vector<Server*> listOfServer;
//    = confParser.parseConfigFile();
    for (std::vector<Server*>::iterator it = listOfServer.begin(); it != listOfServer.end(); ++it)
    {
        std::cout<<RED << "SERVER_NAME: " << it->getServerName() << std::endl;
        std::cout << "IP: " << it->getIp() <<RESET_COLOR <<std::endl;
//        std::cout << "SERVER_NAME: " <<  << std::endl;

    }
    std::cout<<"now we are here "<<std::endl;

//understand if unnecessary and (where setup server) it could be directly replaced with webserver.runEpoll()
//    if(tryToStart(webserver))
//    {
//        std::cout<<"failed to timeStart"<<std::endl;
//        exit(0);//understand if is good to exit or no
//    }

}

//int main(int argc, char** argv)
//{
//    (void ) argc;
//    (void ) argv;
//    std::cout<<"bella"<<std::endl;
//
//    /*
//     * check arg error
//     * check config file ?
//     * call parser to take info from config file and create server object to fullfill --> std::vector<Server> listOfServer
//     * call setup function to  initialize everything
//     * call run (function with main loop)---
//     * */
//    std::map<short,std::string> err;
//    err[404]="not found";
//    err[104]="magari";
//    err[404]="not found";
//    std::vector<short> method(4);
//    std::vector<std::string> cgipath;
//    cgipath[0]="cgipath";
//    std::vector<std::string> cgiext;
//    cgiext[0]="cgiext";
//    //the idea is the every position of the vector is bound to the enum method and if there is 1 is allowed vice versa not allowed
//    for(int i =0;i<4;i++)
//        method[i]=1;
//    Location location("path","root",true,"index",method ,"return","alias",cgipath,cgiext,8);
//    std::vector<Location> locationVector;
//    locationVector.push_back(location);
//    char * ip=(char*)"127.0.0.1";
//    std::vector<config>listOfConfig;
//    config pippo(htons(8080), ip, (std::string &) "prova", (std::string &) "", (std::string &) "", 1000, true, err,locationVector);
//    listOfConfig.push_back(pippo);
//    tryToStart(listOfConfig);
//    //example
////    TODO maybe this function could be implemented in Webserver class
////    Server server1;
////    server1.setupServer();
////    listOfServer.push_back(server1);
//    //end
////
////    Server webserver;
////
////    webserver.setup(listOfServer);
////    webserver.run(listOfServer);
//
//
//
//	return (0);
//}
