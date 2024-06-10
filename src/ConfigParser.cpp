//
// Created by test on 6/9/24.
//

#include "../include/ConfigParser.hpp"


std::vector<Location> vectorOfLocations;

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

unsigned int countIsSpace(const char* str) {
    unsigned int count = 0;

    while (*str) {
        if (std::isspace(*str)) {
            count++;
        }
        str++;
    }
    return count;
}

char findLastChar(std::vector<std::string> matrix) {
    size_t last = matrix.size() - 1;
    char character =  matrix[last][matrix[last].length() - 1];

    return character;
}

std::string trim(std::string str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) {
        start++;
    }

    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    return str.substr(start, end - start);
}

std::string parseLine(std::string line) {
    std::string out;
    size_t commentStart = line.find('#');

    if (commentStart != std::string::npos) {
        out = line.substr(0, commentStart);
    } else {
        out = line;
    }

    out = trim(out);
    if (out.length() == countIsSpace(out.c_str()))
        return "";
    return out;
}

std::vector<std::string> tokenize(std::string s, std::string del = " ")
{
    std::vector<std::string>    tokens;
    size_t start = 0, end = -1 * del.size();

    while ((end = s.find(del, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + del.size();
    }

    if (start < s.length()) {
        tokens.push_back(s.substr(start));
    }

    return tokens;
}

std::string trimLastChar(std::string line) {
    std::string newString;

    for (size_t i = 0; i < line.length() - 1; i++) {
        newString += line[i];
    }
    return newString;
}

std::vector<std::string> AKAftSplit(std::string& str) {
    std::vector<std::string>    tokens;
    std::string                 temp;

    temp = trim(str);
    tokens = tokenize(temp, " ");
    return tokens;
}

/*
 * First state to start cleaning configuration file
 * */
std::string preProcessConfig(std::string conf) {
    std::ifstream   clean_file(conf.c_str());
    std::string     line;
    std::string     out;
    int             counter;

    counter = 0;
    if (clean_file.is_open()) {
        while (std::getline(clean_file, line)) {
            line = parseLine(line);
            if (!line.empty()) {
                out += line + '\n';
            }
            for (size_t i = 0; i < line.length(); ++i) {
                char c = line[i];
                if (c == '{') {
                    ++counter;
                } else if (c == '}') {
                    --counter;
                    if (counter < 0) {
                        std::cerr << "Error : Unbalanced curly braces in line." << std::endl;
                    }
                }
            }
        }
        clean_file.close();
    }

    if (counter != 0) {
        return "CONF ERROR";
    } else {
        return out;
    }
}

/*
 * Function to monitor the state of line in config file
 * */
void ConfigParser::stateCheck(std::string line) {
    if (line.find("http {") != std::string::npos) {
        this->currentState = HttpState;
    } else if (line.find("server {") != std::string::npos) {
        Server server;
        this->currentState = ServerState;
        this->insideServerBlock = true;
        this->_vectorOfLocations.clear();
        this->_vectorOfLocations.resize(0);
        this->countServerBlocks = this->_vectorOfServers.size();
        this->_vectorOfServers.resize(countServerBlocks);
        this->_vectorOfServers.push_back(server);
        this->_vectorOfServers[countServerBlocks].set_location_number(0);
        // printf("server - countserverblocks|countlocationblocks|&server|&_vecOfServers =====> |%d|%d|\n", countServerBlocks, countLocationBlocks, &server, &this->_vectorOfServers);
    } else if (line.find("location") != std::string::npos && this->insideServerBlock) {
        Location *location =new Location();
        this->currentState = LocationState;
        this->countLocationBlocks = this->_vectorOfLocations.size();
        this->_vectorOfServers[countServerBlocks].setIsLocation(true);
        this->_vectorOfServers[countServerBlocks].set_location_number(countLocationBlocks + 1);
        this->_vectorOfLocations.resize(countLocationBlocks);
        this->_vectorOfLocations.push_back(location);
        // printf("location - locations.size() =====> |%d|\n", this->_vectorOfLocations.size());
        printf("location - countserverblocks|countlocationblocks|&location|&getLocations()[i] =====> |%d|%d|%p|%p|\n", countServerBlocks, countLocationBlocks, &location, &this->_vectorOfLocations[countLocationBlocks]);
    }
}

/*
 * Wrapper to switch correct state
 * */
void    ConfigParser::handleLine(std::string line) {
    this->stateCheck(line);

    switch (this->currentState) {
        case HttpState:
            handleHttpState(line);
            break ;
        case ServerState:
            handleServerState(line);
            break ;
        case LocationState:
            handleLocationState(line);
            break ;
        default:
            break;
    }
}

/*
 * State HTTP
 * */
void    ConfigParser::handleHttpState(std::string line) {
    std::vector<std::string> tokens = AKAftSplit(line);

    if (tokens.size() == 1 && tokens[0] == "}") {
        this->currentState = End;
        return ;
    } else if (tokens.size() == 2 && tokens[1] == "{") {
        this->currentState = ServerState;
        return ;
    } else {
        std::cerr << "Error: Http block got wrong configuration" << std::endl;
        exit(2);
    }
}


/*
 * State Server
 * */
void    ConfigParser::handleServerState(std::string line) {
    std::vector<std::string> tokens = AKAftSplit(line);

    if (tokens.size() == 1 && tokens[0] == "}") {
        this->insideServerBlock = false;
        this->currentState = HttpState;
        this->_vectorOfServers[countServerBlocks].initSock();
        this->_vectorOfServers[countServerBlocks].setLocations(this->_vectorOfLocations);
        return ;
    } else if (tokens.size() == 2) {
        if (tokens[1] == "{") {
            return ;
        } else if (tokens[1].back() == ';') {
            tokens[1].pop_back();
            if (tokens[0] == "server_name") {
                this->_vectorOfServers[countServerBlocks].setServerName(tokens[1]);
            } else if (tokens[0] == "listen") {
                this->_vectorOfServers[countServerBlocks].setPort(stoi(tokens[1]));
            } else if (tokens[0] == "autoindex") {
                this->_vectorOfServers[countServerBlocks].setAutoindex(true);
            } else if (tokens[0] == "index") {
                this->_vectorOfServers[countServerBlocks].setIndex(tokens[1]);
            } else if (tokens[0] == "root") {
                this->_vectorOfServers[countServerBlocks].setRoot(tokens[1]);
            } else if (tokens[0] == "body_size") {
                this->_vectorOfServers[countServerBlocks].setClientMaxBodySize(tokens[1]);
            } else if (tokens[0] == "host") {
                this->_vectorOfServers[countServerBlocks].setIp(tokens[1]);
            } else if (!tokenAdmitted(tokens[0])) {
                std::cout << "Error: wrong error page in server block from config file, found: " << tokens[0] << std::endl;
                exit(2);
            }
        }
    } else if (tokens.size() > 2 && tokens[0] == "error_page" && findLastChar(tokens) == ';') {
        tokens[tokens.size() - 1] = trimLastChar(tokens[tokens.size() - 1]);
        this->_vectorOfServers[countServerBlocks].setErrorPages(tokens);
    } else if (tokens.size() > 2 && tokens[0] == "cgi_enable" && findLastChar(tokens) == ';') {
        tokens[tokens.size() - 1] = trimLastChar(tokens[tokens.size() - 1]);
        // this->_vectorOfServers[countServerBlocks].setCgiPath(tokens); /////////////////
    } else {
        std::cerr << "Error: Server block got wrong configuration" << std::endl;
    }
}

/*
 * State Location
 * */
void    ConfigParser::handleLocationState(std::string line) {
    std::vector<std::string> tokens = AKAftSplit(line);

    if (tokens.size() == 1 && tokens[0] == "}") {
        this->currentState = ServerState;
        return ;
    } else if (tokens.size() == 2 && tokens[0] != "method" && tokens[0] != "return" && tokens[1].back() == ';') {
        tokens.pop_back();
        if (tokens[0] == "autoindex") {
            this->_vectorOfLocations[countLocationBlocks]->setAutoIndex(true);
        } else if (tokens[0] == "index") {
            this->_vectorOfLocations[countLocationBlocks]->setIndex(tokens[1]);
        } else if (tokens[0] == "root") {
            this->_vectorOfLocations[countLocationBlocks]->setRoot(tokens[1]);
        } else if (tokens[0] == "body_size") {
            this->_vectorOfLocations[countLocationBlocks]->setClientMaxBodySize(tokens[1]);
        } else if (!tokenAdmitted(tokens[0])) {
            std::cout << "Error: wrong configuration in location block from config file, found: " << tokens[0] << std::endl;
            exit(2);
        }
    } else if (tokens.size() == 3 && tokens[0] == "location" && tokens[2] == "{") {
        this->_vectorOfLocations[countLocationBlocks]->setPath(tokens[1]);
    } else if (tokens[0] == "method" && findLastChar(tokens) == ';') {
        tokens[tokens.size() - 1] = trimLastChar(tokens[tokens.size() - 1]);
        this->_vectorOfLocations[countLocationBlocks]->setMethods(tokens);
    } else if (tokens[0] == "return" && findLastChar(tokens) == ';') {
        tokens[tokens.size() - 1] = trimLastChar(tokens[tokens.size() - 1]);
        this->_vectorOfLocations[countLocationBlocks]->setReturn(tokens);
    }
}

/*
 * Debug purpose
 * */
void ConfigParser::printConfig() {
    // Print HTTP block information
    std::cout << MAGENTA << "\n*** HTTP Block ***\n" << RESET_COLOR;

    // Print server block information
    std::cout << GREEN << "\n*** Server Blocks ***\n" << RESET_COLOR;
    std::vector<Server> vectorOfServers = this->_vectorOfServers;
    for (std::vector<Server>::iterator it = vectorOfServers.begin(); it != vectorOfServers.end(); ++it) {
        std::cout << GREEN <<  "\n  *** Server Block ***\n" << RESET_COLOR;
        std::cout << GREEN <<  "      SERVER_NAME: " << it->getServerName() << RESET_COLOR << std::endl;
        std::cout << GREEN <<  "      PORT: " << it->getPort() << RESET_COLOR << std::endl;
        std::cout << GREEN <<  "      IP: " << it->getIp() << RESET_COLOR << std::endl;
        std::cout << GREEN <<  "      ROOT: " << it->getRoot() << RESET_COLOR << std::endl;
        std::cout << GREEN <<  "      INDEX: " << it->getIndex() << RESET_COLOR << std::endl;
        std::cout << GREEN <<  "      CLIENT-MAX-BODY-SIZE: " << it->getClientMaxBodySize() << RESET_COLOR << std::endl;
        // Print error pages for this server block
        if (!it->getErrorPages().empty()) {
            std::cout << GREEN << "      Default error pages: " << RESET_COLOR;
            for (std::vector<std::string>::iterator it22 = it->getErrorPages().begin(); it22 != it->getErrorPages().end(); ++it22) {
                std::cout << GREEN << *it22 << " " << RESET_COLOR;
            }
            std::cout << std::endl;
        }


        // Print location blocks information within this server block
        if (!it->getLocations().empty()) {
            std::vector<Location*> locationVec = it->getLocations();
            for (std::vector<Location*>::iterator it3 = locationVec.begin(); it3 != locationVec.end(); ++it3) {
                std::cout << BLUE <<  "\n      *** Location Block ***\n" << RESET_COLOR;
                std::cout << BLUE << "        PATH: " << (*it3)->getPath() << RESET_COLOR << std::endl;
                std::cout << BLUE << "        ROOT: " << (*it3)->root() << RESET_COLOR << std::endl;
                std::cout << BLUE << std::boolalpha << "        AUTOINDEX: " << (*it3)->getAutoIndex() << RESET_COLOR << std::endl;
                std::cout << BLUE << "        ALIAS: " << (*it3)->alias() << RESET_COLOR << std::endl;
                // Print error pages for this location block
                if (!(*it3)->getReturn().empty()) {
                    std::vector<std::string> returnVec = (*it3)->getReturn();
                    std::cout << BLUE << "        Error Pages: " << RESET_COLOR;
                    for (std::vector<std::string>::iterator it5 = returnVec.begin(); it5 != returnVec.end(); ++it5) {
                        std::cout << BLUE << *it5 << " " << RESET_COLOR;
                    }
                    std::cout << std::endl;
                }
                if (!(*it3)->getMethods().empty()) {
                    std::vector<std::string> methods = (*it3)->getMethods();
                    std::cout << BLUE << "        Methods: " << RESET_COLOR;
                    for (std::vector<std::string>::iterator it6 = methods.begin(); it6 != methods.end(); ++it6) {
                        std::cout << BLUE << *it6 << " " << RESET_COLOR;
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    std::cout << "\n\n\n\n";
}

/*
 * Parse config file and relative checks
 * return List Of Servers
 * */
std::vector<Server> ConfigParser::parseConfigFile() {
    std::string         line;
    std::string         out;

    this->_parsedConfig = preProcessConfig(this->_configFile);
    printf("%s\n", this->_parsedConfig.c_str()); /////////// debug
    if (this->_parsedConfig == "CONF ERROR") {
        std::cerr << "Found error in conf file.\n" << this->_parsedConfig << std::endl;
        exit(2);
    }

    std::stringstream   file(this->_parsedConfig);
    if (file.bad()) {
        std::cerr << "Error manipulating file:\n" << this->_parsedConfig << std::endl;
        exit(2);
    }
    this->currentState = -1;
    while (std::getline(file, line)) {
        this->handleLine(line);
    }

    //////////
    this->printConfig();
    //////////

    // for (std::vector<Server>::iterator vecSer = _vectorOfServers.begin(); vecSer != _vectorOfServers.end(); ++vecSer) {
    //     std::cout << "SERVER_NAME: " << vecSer->getServerName() << std::endl;
    //     if (!vecSer->getLocations().empty()) {
    //         for (std::vector<Location>::iterator vecLoc = vecSer->getLocations().begin(); vecLoc != vecSer->getLocations().end(); ++vecLoc) {
    //             std::cout << "LOCATION_PATH: " << vecLoc->getPath() << std::endl;
    //         }
    //     }
    // }

    return this->_vectorOfServers;
}
