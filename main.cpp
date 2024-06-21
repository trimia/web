#include "include/ConfigParser.hpp"
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
    std::signal(SIGINT, sigint_handler);
    if(webserver.runEpoll())
        return 0;
    return 1;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: wrong number of arguments." << std::endl;
        exit(2);
    }
    Webserver webserver(argv[1]);
    ws_ptr=&webserver;
    if(tryToStart(webserver))
    {
        std::cout<<RED<<"failed to timeStart"<<RESET_COLOR<<std::endl;
    }

}
