#include "../include/ConfigParser.hpp"
#include "../include/ConfigBlock.hpp"
#include <sstream>
#include <stdio.h>


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
 * Function to monitor the state of line in config file
 * */
void ConfigParser::stateCheck(std::string line) {
    if (line.find("http {") != std::string::npos) {
        this->currentState = HttpState;
        std::cout << "\t\t\t\t\t\t\t\t\t\tHTTP" << std::endl;
    } else if (line.find("server {") != std::string::npos) {
        this->currentState = ServerState;
        this->insideServerBlock = true;
        this->_configBlock.getServerBlocks().resize(this->countServBlocks + 1);
        std::cout << "\t\t\t\t\t\t\t\t\t\tSERVER" << std::endl;
    } else if (line.find("location") != std::string::npos && this->insideServerBlock) {
        this->currentState = LocationState;
        std::cout << "\t\t\t\t\t\t\t\t\t\tLOCATION" << std::endl;
        this->_configBlock.getServerBlocks()[countServBlocks]
            .locationBlock.resize(this->countLocBlocks + 1);
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
    std::vector<std::string> lineToks = AKAftSplit(line);

    if (lineToks.size() == 1 && lineToks[0] == "}") {
        this->currentState = End;
        return ;
    } else if (lineToks.size() == 2) {
        if (lineToks[1] == "{") {
            this->currentState = ServerState;
            return ;
        } else {
            if (lineToks[1][lineToks[1].length() - 1] == ';') {
                lineToks[1] = trimLastChar(lineToks[1]);
                this->_configBlock.getHttpBlock().keyValue
                .insert(std::make_pair(lineToks[0], lineToks[1]));
            }
        }
    } else if (lineToks.size() == 3) {
        this->_configBlock.getHttpBlock().errorPages = lineToks;
    } else {
        std::cerr << "Error: Http block got wrong configuration" << std::endl;
    }
}


/*
 * State Server
 * */
void    ConfigParser::handleServerState(std::string line) {
    std::vector<std::string> lineToks = AKAftSplit(line);

    if (lineToks.size() == 1 && lineToks[0] == "}") {
        this->insideServerBlock = false;
        this->currentState = HttpState;
        this->countServBlocks += 1;
        return ;
    } else if (lineToks.size() == 2) {
        if (lineToks[1] == "{") {
            return ;
        } else {
            if (lineToks[1][lineToks[1].length() - 1] == ';') {
                lineToks[1] = trimLastChar(lineToks[1]);
                this->_configBlock.getServerBlocks()[countServBlocks]
                    .keyValue.insert(std::make_pair(lineToks[0], lineToks[1]));
            }
        }
    } else if (lineToks.size() > 2 && lineToks[0] == "error_page") {
        size_t last = lineToks.size() - 1;
        if (lineToks[last][lineToks[last].length() - 1] == ';') {
            this->_configBlock.getServerBlocks()[countServBlocks].errorPages = lineToks;
        }
    } else if (lineToks.size() > 2 && lineToks[0] == "cgi_enable") {
        size_t last = lineToks.size() - 1;
        if (lineToks[last][lineToks[last].length() - 1] == ';') {
            this->_configBlock.getServerBlocks()[countServBlocks].cgiPath = lineToks;
        }
    } else {
        std::cerr << "Error: Server block got wrong configuration" << std::endl;
    }
}

/*
 * State Location
 * */
void    ConfigParser::handleLocationState(std::string line) {
    std::vector<std::string> lineToks = AKAftSplit(line);

    if (lineToks.size() == 1 && lineToks[0] == "}") {
        this->currentState = ServerState;
        this->countLocBlocks += 1;
    } else if (lineToks.size() == 2) {
        if (lineToks[1][lineToks[1].length() - 1] == ';') {
            lineToks[1] = trimLastChar(lineToks[1]);
            this->_configBlock.getServerBlocks()[countServBlocks]
                .locationBlock[countLocBlocks].keyValue
                .insert(std::make_pair(lineToks[0], lineToks[1]));
        }
    } else if (lineToks.size() == 3) {
        if (lineToks[0] == "return") {
//            size_t last = lineToks.size() - 1;
//
//            if (lineToks[last][lineToks[last].length() - 1] == ';') {
//                lineToks[last] = trimLastChar(lineToks[last]);
                this->_configBlock.getServerBlocks()[countServBlocks]
                        .locationBlock[countLocBlocks].retErrorPages = lineToks;
//                printf("%s\n", this->_configBlock.getServerBlocks()[countServBlocks] //////////// check this case
//                        .locationBlock[countLocBlocks].retErrorPages[0].c_str());
//            }
        } else if (lineToks[0] == "location" && lineToks[2] == "{") {
            this->_configBlock.getServerBlocks()[countServBlocks]
                .locationBlock[countLocBlocks].keyValue
                .insert(std::make_pair(lineToks[0], lineToks[1]));
        }
    } if (lineToks[0] == "method") {
            this->_configBlock.getServerBlocks()[countServBlocks]
                .locationBlock[countLocBlocks].methods = lineToks;
//    } else {
//        std::cerr << "Error: Location block got wrong configuration" << std::endl;
    }
}

/*
 * Debug purpose
 * */
void ConfigParser::printConfig() {
    // Print HTTP block information
    std::cout << "\n*** HTTP Block ***\n";
    std::map<std::string, std::string> httpBlock = _configBlock.getHttpBlock().keyValue;
    for (std::map<std::string, std::string>::iterator it = httpBlock.begin();
         it != httpBlock.end(); ++it) {
        std::cout << "    Key: " << it->first << ", Value: " << it->second << std::endl;
    }

    // Print server block information
    std::cout << "\n*** Server Blocks ***\n";
    for (std::vector<ServerBlock>::iterator it = _configBlock.getServerBlocks().begin();
         it != _configBlock.getServerBlocks().end(); ++it) {
        std::cout << "\n  *** Server Block ***\n";
        std::map<std::string, std::string> serverBlockKV = it->keyValue;
        for (std::map<std::string, std::string>::iterator it2 = serverBlockKV.begin();
             it2 != serverBlockKV.end(); ++it2) {
            std::cout << "      Key: " << it2->first << ", Value: " << it2->second << std::endl;
        }
        // Print error pages for this server block
        if (!it->errorPages.empty()) {
            std::cout << "      Default error pages:";
            for (std::vector<std::string>::iterator it22 = it->errorPages.begin();
                 it22 != it->errorPages.end(); ++it22) {
                std::cout << *it22 << " ";
            }
            std::cout << std::endl;
        }


        // Print location blocks information within this server block
        for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin();
             it3 != it->locationBlock.end(); ++it3) {
            std::cout << "\n    *** Location Block ***\n";
            std::map<std::string, std::string> locationBlockKV = it3->keyValue;
            for (std::map<std::string, std::string>::iterator it4 = locationBlockKV.begin();
                 it4 != locationBlockKV.end(); ++it4) {
                std::cout << "        Key: " << it4->first << ", Value: " << it4->second << std::endl;
            }
            // Print error pages for this location block
            if (!it3->retErrorPages.empty()) {
                std::cout << "        Error Pages: ";
                for (std::vector<std::string>::iterator it5 = it3->retErrorPages.begin();
                     it5 != it3->retErrorPages.end(); ++it5) {
                    std::cout << *it5 << " ";
                }
                std::cout << std::endl;
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

    this->_parsed_config = preProcessConfig(this->_config_file);
    printf("%s\n", this->_parsed_config.c_str());
    if (this->_parsed_config == "CONF ERROR") {
        std::cerr << "Found error in conf file.\n" << this->_parsed_config << std::endl;
        exit(2);
    }

    std::stringstream   file(this->_parsed_config);
    if (file.bad()) {
        std::cerr << "Error manipulating file:\n" << this->_parsed_config << std::endl;
        exit(2);
    }
    this->currentState = -1;
    while (std::getline(file, line)) {
        this->handleLine(line);
    }

    this->printConfig();

    //////////
    return this->_configBlock.handleBlock();
}

//void    ConfigParser::extractKeyword() {
//    std::stringstream   file(this->_parsed_config);
//    std::string         line;
//    std::string         out;
//
//    if (file.bad()) {
//        std::cerr << "Error manipulating file:\n" << this->_parsed_config << std::endl;
//        return;
//    }
//
//    this->currentState = -1;
//    while (std::getline(file, line)) {
//        this->handleLine(line);
//    }
//
//    this->printConfig();
//    this->getConfigBlock().handleBlock();
//}
//
//void ConfigParser::parseConfigFile() {
//
//    this->_parsed_config = getParsedConfig(this->_config_file);
//    if (this->_parsed_config == "CONF ERROR") {
//        std::cerr << "Found error in conf file.\n" << this->_parsed_config << std::endl;
//        exit(1);
//    } else {
//        this->extractKeyword();
//    }
//}
