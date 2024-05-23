#pragma once
#ifndef INCLUDE_H
# define INCLUDE_H
# include <iostream>
// # include <fstream>
# include <fcntl.h>
# include <cstring>
# include <string>
# include <unistd.h>
# include <dirent.h>
# include <sstream>
// # include <bits/stdc++.h>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <cstdarg>

/* STL Containers */
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>

/* System */
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>

// # include <machine/types.h>
//# include <signal.h>
# include "csignal"

/* Network */
# include <sys/socket.h>
# include <pcap/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <fcntl.h>

# define BUFFER_SIZE 4096
# define ISVALIDSOCKET(s) ((s) >= 0)
# define CLOSESOCKET(s) close(s)
# define SOCKET_ERROR -1
//# define RCV_BUF_SIZE (MAX_HTTP_HEAD_LINE > MAX_HTTP_REQ_LINE ? MAX_HTTP_HEAD_LINE : MAX_HTTP_REQ_LINE)
# define RCV_BUF_SIZE 8000
# define MAX_N_CONNECTION 5
// MAX_EVENT AND EPOLL_SIZE maybe they could be defined every time after parsing so maybe became part of a struct or class no more a macro
# define MAX_EVENTS 1024
# define EPOLL_TIMEOUT 2000
# define SERVER_SOCK 1
# define CLIENT_SOCK 2
//# define EPOLL_SIZE 1000
# define BUFFER 40000
# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define MAGENTA	"\033[0;35m"
# define CYAN		"\033[0;36m"
# define WHITE		"\033[0;37m"

# define RESET_COLOR "\033[0m"
# define DEFAULT_ERR_DIR "error_pages/"
# define MIMETYPES_FILE_PATH "assets/mimeTypes.csv" //???

struct sType
{
//    sType();
//    sType(int n):socketType(n){}
    int     socketType;
    sType() { socketType=0;}
};
//understand if is necessary or could be deleted
struct sConnection :public sType
{
    bool            error;
    bool            cgi;
    bool            ended;
    std::time_t     timeStart;

    sConnection(){ timeStart=0;error=false;cgi= false;}
};

enum e_resource_type
{
    RT_FILE,
    RT_DIR
};
enum HttpMethod
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

//other file
// # include "Location.hpp"
// # include "Request.hpp"
// # include "Response.hpp"
// # include "Socket.hpp"
// # include "Server.hpp"
// # include "Client.hpp"
// # include "Webserver.hpp"
//class Location;
//class config;
//class Request;
//class Response;
//class Server;
//class client;
//class Webserver;
//class Parser;
// class Socket;
extern int g_sigint; //????

#endif
