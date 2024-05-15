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

/////// static ?
Server  handleServerBlock(std::map<std::string, std::string> keyValue, std::vector<std::string> vecString) {
    Server  server;

    (void)vecString; //////////////////////// fix it
    for (std::map<std::string, std::string>::iterator it = keyValue.begin();
            it != keyValue.end(); ++it) {
//        printf("K %s\tV %s\n", it->first.c_str(), it->second.c_str());
            if (it->first == "server_name") {
                server.setServerName(it->second);
            } else if (it->first == "listen") {
                server.setPort(stoi(it->second));
            } else if (it->first == "autoindex") {
                server.setAutoindex(true); // check how to handle autoindex
            } else if (it->first == "index") {
                server.setIndex(it->second);
            } else if (it->first == "root") {
                server.setRoot(it->second);
            } else if (it->first == "body_size") {
                server.setClientMaxBodySize(it->second);
            } else if (it->first == "host") {
                //if we do _ip const we elide the cast but don't ḱnow if is good for htons()
                server.setIp((char *) it->second.c_str());
//                printf("-------------------- %s\n", server.getIp().c_str());
//            } if (vecString[0] == "error_page") {
//                    server.setErrorPages(vecString);
//            } else {
//                std::cout << "Error: wrong error page in server block from config file" << std::endl;
//                exit(2);
            }
    }

//    printf("\n\n%s|%s| |%s| |%hu| %s\n\n", BLUE, server.getServerName().c_str(), server.getIp().c_str(), server.getPort(), RESET_COLOR);
    return server;
}

Location    handleLocationBlock(std::map<std::string, std::string> keyValue, std::vector<std::string> methods, std::vector<std::string> errPage) {
    //////////// cgi_path ???
    (void)errPage;
    Location location;

    for (std::map<std::string, std::string>::iterator it = keyValue.begin();
         it != keyValue.end(); ++it) {
//        printf("KL %s\tVL %s\n", it->first.c_str(), it->second.c_str());
        if (it->first == "location") {
            location.setPath(it->second);
        } else if (it->first == "autoindex") {
            location.setAutoIndex(true); // check how to handle autoindex
        } else if (it->first == "index") {
            location.setIndex(it->second);
        } else if (it->first == "root") {
            location.setRoot(it->second);
        } else if (it->first == "body_size") {
            location.setClientMaxBodySize(it->second);
        } if (methods[0] == "method") {
                location.setMethods(methods);
//        } if (errPage[0] == "return") {
//                    location.setReturn(errPage);
        }
//            }
//        } else {
//            std::cout << "Error: wrong configuration in location block from config file" << std::endl;
//            exit(2);
//        }
    }
    return location;
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

    Server                  server;
    Location                location;
    std::vector<Location>   listOfLocation;
    std::vector<Server>     listOfServers;

    ///////////////// check if we need to free space
    // listOfServers.reserve(countServBlocks);
    // listOfLocation.reserve(countLocBlocks);
    for (std::vector<ServerBlock>::iterator it = getServerBlocks().begin();
        it != getServerBlocks().end();++ it) {

        listOfLocation.clear();
        for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin();
             it3 != it->locationBlock.end(); ++it3) {
            location = handleLocationBlock(it3->keyValue, it3->methods, it3->retErrorPages);
            listOfLocation.push_back(location);
        }

        server.setLocations(listOfLocation);
        server = handleServerBlock(it->keyValue, it->errorPages);
        //trimia: this necessary for every server to setup socket tell me if is the right place
        server.initSock();
        //
        listOfServers.push_back(server);
    }

    return listOfServers;
}