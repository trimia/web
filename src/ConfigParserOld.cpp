#include "../include/ConfigParser.hpp"
// #include "../include/ConfigBlock.hpp"
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
    } else if (line.find("server {") != std::string::npos) {
        this->currentState = ServerState;
        this->insideServerBlock = true;
        this->_configBlock.getServerBlocks().resize(this->countServBlocks + 1);
    } else if (line.find("location") != std::string::npos && this->insideServerBlock) {
        this->currentState = LocationState;
        this->_configBlock.getServerBlocks()[countServBlocks].locationBlock.resize(this->countLocBlocks + 1);
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
        } else if (findLastChar(lineToks) == ';') {
                lineToks[1] = trimLastChar(lineToks[1]);
                this->_configBlock.getServerBlocks()[countServBlocks]
                .keyValue.insert(std::make_pair(lineToks[0], lineToks[1]));
        }
    } else if (lineToks.size() > 2 && lineToks[0] == "error_page" && findLastChar(lineToks) == ';') {
        lineToks[lineToks.size() - 1] = trimLastChar(lineToks[lineToks.size() - 1]);
        this->_configBlock.getServerBlocks()[countServBlocks].errorPages = lineToks;
    } else if (lineToks.size() > 2 && lineToks[0] == "cgi_enable" && findLastChar(lineToks) == ';') {
        lineToks[lineToks.size() - 1] = trimLastChar(lineToks[lineToks.size() - 1]);
        this->_configBlock.getServerBlocks()[countServBlocks].cgiPath = lineToks;
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
//        this->countLocBlocks += 1;
    } else if (lineToks.size() == 2 && lineToks[0] != "method" && lineToks[0] != "return") {
        if (lineToks[1][lineToks[1].length() - 1] == ';') {
            lineToks[1] = trimLastChar(lineToks[1]);
            this->_configBlock.getServerBlocks()[countServBlocks]
                .locationBlock[countLocBlocks].keyValue
                .insert(std::make_pair(lineToks[0], lineToks[1]));
        }
    } else if (lineToks.size() == 3 && lineToks[0] == "location" && lineToks[2] == "{") {
        this->_configBlock.getServerBlocks()[countServBlocks]
                .locationBlock[countLocBlocks].keyValue
                .insert(std::make_pair(lineToks[0], lineToks[1]));
    }
    if (lineToks[0] == "method" && findLastChar(lineToks) == ';') {
        lineToks[lineToks.size() - 1] = trimLastChar(lineToks[lineToks.size() - 1]);
        this->_configBlock.getServerBlocks()[countServBlocks].locationBlock[countLocBlocks].methods = lineToks;
    }
    if (lineToks[0] == "return" && findLastChar(lineToks) == ';') {
        lineToks[lineToks.size() - 1] = trimLastChar(lineToks[lineToks.size() - 1]);
        this->_configBlock.getServerBlocks()[countServBlocks].locationBlock[countLocBlocks].retErrorPages = lineToks;
    }
}

/*
 * Debug purpose
 * */
void ConfigParser::printConfig() {
    // Print HTTP block information
    std::cout << YELLOW << "\n*** HTTP Block ***\n" << RESET_COLOR;
    std::map<std::string, std::string> httpBlock = _configBlock.getHttpBlock().keyValue;
    for (std::map<std::string, std::string>::iterator it = httpBlock.begin();
         it != httpBlock.end(); ++it) {
        std::cout << YELLOW << "    Key: " << it->first << ", Value: " << it->second << RESET_COLOR << std::endl;
    }

    // Print server block information
    std::cout << GREEN << "\n*** Server Blocks ***\n" << RESET_COLOR;
    for (std::vector<ServerBlock>::iterator it = _configBlock.getServerBlocks().begin(); it != _configBlock.getServerBlocks().end(); ++it) {
        std::cout << GREEN <<  "\n  *** Server Block ***\n" << RESET_COLOR;
        std::map<std::string, std::string> serverBlockKV = it->keyValue;
        for (std::map<std::string, std::string>::iterator it2 = serverBlockKV.begin(); it2 != serverBlockKV.end(); ++it2) {
            std::cout << GREEN <<  "      Key: " << it2->first << ", Value: " << it2->second << RESET_COLOR << std::endl;
        }
        // Print error pages for this server block
        if (!it->errorPages.empty()) {
            std::cout << GREEN << "      Default error pages: " << RESET_COLOR;
            for (std::vector<std::string>::iterator it22 = it->errorPages.begin(); it22 != it->errorPages.end(); ++it22) {
                std::cout << GREEN << *it22 << " " << RESET_COLOR;
            }
            std::cout << std::endl;
        }


        // Print location blocks information within this server block
        if (!it->locationBlock.empty()) {
            for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin(); it3 != it->locationBlock.end(); ++it3) {
                std::cout << BLUE << "\n    *** Location Block ***\n" << RESET_COLOR;
                std::map<std::string, std::string> locationBlockKV = it3->keyValue;
                for (std::map<std::string, std::string>::iterator it4 = locationBlockKV.begin(); it4 != locationBlockKV.end(); ++it4) {
                    std::cout << BLUE << "        Key: " << it4->first << ", Value: " << it4->second << RESET_COLOR << std::endl;
                }
                // Print error pages for this location block
                if (!it3->retErrorPages.empty()) {
                    std::cout << BLUE << "        Error Pages: " << RESET_COLOR;
                    for (std::vector<std::string>::iterator it5 = it3->retErrorPages.begin(); it5 != it3->retErrorPages.end(); ++it5) {
                        std::cout << BLUE << *it5 << " " << RESET_COLOR;
                    }
                    std::cout << std::endl;
                }
                if (!it3->methods.empty()) {
                    std::cout << BLUE << "        Methods: " << RESET_COLOR;
                    for (std::vector<std::string>::iterator it6 = it3->methods.begin(); it6 != it3->methods.end(); ++it6) {
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

    this->_parsed_config = preProcessConfig(this->_config_file);
    printf("%s\n", this->_parsed_config.c_str()); /////////// debug
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

    //////////
    this->printConfig();
    //////////

    return this->_configBlock.handleBlock();
}
