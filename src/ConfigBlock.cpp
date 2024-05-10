#include "../include/ConfigBlock.hpp"
#include "../include/ConfigParser.hpp"
#include <sstream>


/*
 * Implement the simil Data Transfer Object
 * sends from parsed block to relative Object
 * */

static uint16_t stoi(std::string& s) {
    char*       endptr;
    long int    parsedValue = strtol(s.c_str(), &endptr, 10);

    if (endptr == s.c_str() || *endptr != '\0') {
        return 0;
    }
    return static_cast<uint16_t>(parsedValue);
}

void    ConfigBlock::handleHTTPBlock() {
    ;
}

void    ConfigBlock::handleServerBlock(Server server, std::string key, std::string value) {
//  else if (key == "cgi_enable") {
    if (key == "server_name") {
        server.setServerName(value);
    } else if (key == "listen") {
        server.setPort(stoi(value));
    } else if (key == "autoindex") {
        server.setAutoindex(true); // check how to handle autoindex
    } else if (key == "index") {
        server.setIndex(value);
    } else if (key == "root") {
        server.setRoot(value);
    } else if (key == "body_size") {
        server.setClientMaxBodySize(value);
    } else if (key == "host") {
        //if we do _ip const we elide the cast but don't ḱnow if is good for htons()
        server.setIp((char *)value.c_str());
    } else {
        std::cout << "Error: wrong server block from config file 1" << std::endl;
        exit(2);
    }
}

void    ConfigBlock::handleLocationBlock(Location location, std::string key, std::string value) {
    //////////// cgi_path ???
    if (key == "location") {
        location.setPath(value);
    } else if (key == "autoindex") {
        location.setAutoIndex(true); // check how to handle autoindex
    } else if (key == "index") {
        location.setIndex(value);
    } else if (key == "root") {
        location.setRoot(value);
    } else if (key == "body_size") {
        location.setClientMaxBodySize(value);
    } else {
        std::cout << "Error: wrong server block from config file 2" << std::endl;
//        trimia commentato perche'manacaanoalcuni parametri e mi serve per mandare aventi il programma
//        exit(2);
    }
}

void    ConfigBlock::handleServVectors(Server server, std::vector<std::string> vecString) {
    //////////// cgi_path ???
    if (vecString[0] == "error_page") {
        server.setErrorPages(vecString);
    } else {
        std::cout << "Error: wrong error page in server block from config file" << std::endl;
        exit(2);
    }
}

void    ConfigBlock::handleLocVectors(Location location, std::vector<std::string> vecString) {
    if (vecString[0] == "method") {
        location.setMethods(vecString);
    } else if (vecString[0] == "return") {
        location.setReturn(vecString);
    } else {
        std::cout << "Error: wrong configuration in location block (method / return)" << std::endl;
        exit(2);
    }
}

/*
 * Wrapper that iterates over each server
 *
 * for server in servers
 *      get_values and set server values
 *      for location in locations
 *          get_values and set location values
 *      set list of locations inside single server
 *      set server inside list of servers
 * */
std::vector<Server> ConfigBlock::handleBlock() {
    Socket                  socket;
    Server                  server;
    Location                location;
    std::vector<Location>   listOfLocation;
    std::vector<Server>     listOfServers;

    // HTTP block information
    std::map<std::string, std::string> httpBlock = getHttpBlock().keyValue;
    for (std::map<std::string, std::string>::iterator it = httpBlock.begin();
        it != httpBlock.end(); ++it) {
        this->handleHTTPBlock();
    }

    // server block information
    size_t countServ = 0;
    for (std::vector<ServerBlock>::iterator it = getServerBlocks().begin();
        it != getServerBlocks().end(); ++it) {
        std::map<std::string, std::string> serverBlockKV = it->keyValue;
        for (std::map<std::string, std::string>::iterator it2 = serverBlockKV.begin();
            it2 != serverBlockKV.end(); ++it2) {
            this->handleServerBlock(server, it2->first, it2->second);
        }
        // error pages for this server block
        if (!it->errorPages.empty()) {
            this->handleServVectors(server, it->errorPages);
        }

        // location blocks information within this server block
        size_t  countLoc = 0;
        for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin();
             it3 != it->locationBlock.end(); ++it3) {

            std::map<std::string, std::string> locationBlockKV = it3->keyValue;
            for (std::map<std::string, std::string>::iterator it4 = locationBlockKV.begin();
                it4 != locationBlockKV.end(); ++it4) {
                this->handleLocationBlock(location, it4->first, it4->second);
            }
            // error pages for this location block
            if (!it3->retErrorPages.empty()) {
                this->handleLocVectors(location, it3->retErrorPages);
            }
            if (!it3->methods.empty()) {
                this->handleLocVectors(location, it3->methods);
            }

            countLoc++;
            listOfLocation.resize(countLoc);
            listOfLocation.push_back(location);
        }
        server.setLocations(listOfLocation);

        ////////////////////////
        //////////////// Implement logic in server class, PLS i'm begging you
        //////////
//        socket.createSocket(server);
//        socket.setSocketOption(server);
//        socket.bindSocket(server);
//        socket.listenOnSocket(server.getServerSocket()->getFdSock());
//        fcntl(server.getServerSocket()->getFdSock(),F_SETFL,O_NONBLOCK);
        ///

        countServ++;
        listOfServers.resize(countServ);
        listOfServers.push_back(server);

    }

    return listOfServers;
}

