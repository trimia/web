#include "../include/ConfigBlock.hpp"
// #include "../include/ConfigParser.hpp"
#include <sstream>

static bool    tokenAdmitted(std::string token) {
    if (token != "host" && token != "listen" && token != "server_name"
        && token != "root" && token != "error_page" && token != "body_size"
        && token != "autoindex" && token != "method" && token != "return"
        && token != "cgi_enable" && token != "index") {
        return false;
    }
    return true;
}

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

static Server  handleServerBlock(std::vector<LocationBlock> it, std::map<std::string, std::string> keyValue, std::vector<std::string> vecString, std::vector<std::string> cgiPath) {
    Server  server;

    server.set_location_number(it.size());

    if (!it.empty())
        server.setIsLocation(true);

    if (!vecString.empty() && vecString[0] == "error_page")
        server.setErrorPages(vecString);

    if (!cgiPath.empty() && cgiPath[0] == "cgi_enable")
        server.setIsCgiEnabled(true);
        // server.setCgiPath();
        // printf("\n\ntokens\n\n|%s| |%s| |%s|\n\n", cgiPath[0].c_str(), cgiPath[1].c_str(), cgiPath[2].c_str());

    for (std::map<std::string, std::string>::iterator it = keyValue.begin();
            it != keyValue.end(); it++) {
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
                server.setIp(it->second);
            } else if (!tokenAdmitted(it->first)) {
                std::cout << "Error: wrong error page in server block from config file, found: " << it->first << std::endl;
                exit(2);
            }
    }

    return server;
}

static Location    handleLocationBlock(std::map<std::string, std::string> keyValue, std::vector<std::string> methods, std::vector<std::string> errPage) {
    Location location;

    //////////// cgi_path ???
    if (!methods.empty() && methods[0] == "method")
        location.setMethods(methods);
    if (!errPage.empty() && errPage[0] == "return")
        location.setReturn(errPage);

    for (std::map<std::string, std::string>::iterator it = keyValue.begin();
         it != keyValue.end(); ++it) {
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
        } else if (!tokenAdmitted(it->first)) {
            std::cout << "Error: wrong configuration in location block from config file, found: " << it->first << std::endl;
            exit(2);
        }
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

    for (std::vector<ServerBlock>::iterator it = getServerBlocks().begin();
        it != getServerBlocks().end(); ++it) {

        listOfLocation.clear();
        for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin();
            it3 != it->locationBlock.end(); ++it3) {
            location = handleLocationBlock(it3->keyValue, it3->methods, it3->retErrorPages);
            listOfLocation.push_back(location);
        }
        server.setLocations(listOfLocation);
        server = handleServerBlock(it->locationBlock, it->keyValue, it->errorPages, it->cgiPath);
        server.initSock();

        listOfServers.push_back(server);
    }

    return listOfServers;
}