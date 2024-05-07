//
// Created by test on 5/1/24.
//

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

/*
 * uint16_t						    _port;
 * char*                            _ip;
 * std::string						_server_name;
 * std::string						_root;
 * std::string						_index;
 * unsigned long					_client_max_body_size;
 * bool							    _autoindex;
 * std::map<short, std::string>	    _error_pages;
 * */
void    ConfigBlock::handleServerBlock(std::string key, std::string value) {
    if (key == "server_name") {
        this->_server.setServerName(value);
    } else if (key == "listen") {
        this->_server.setPort(stoi(value));
    } else if (key == "autoindex") {
//        when enabled autoindex can only be 'on' otherwise error?
        this->_server.setAutoIndex(true);
    }
}

/*
 * std::string					_path;
 * std::string					_root;
 * bool  						_autoindex;
 * std::string					_index;
 * std::vector<short>			_methods; // GET+ POST- DELETE- PUT- HEAD-
 * std::string					_return;
 * std::string					_alias;
 * std::vector<std::string> 	_cgi_path;
 * std::vector<std::string>	    _cgi_ext;
 * unsigned long				_client_max_body_size;
 * */
void    ConfigBlock::handleLocationBlock() {}

void    ConfigBlock::handleServErrorPages() {}
void    ConfigBlock::handleLocErrorPages() {}


void    ConfigBlock::handleBlock() {
    // Print HTTP block information
    std::cout << "\n*** HTTP Block ***\n";
    std::map<std::string, std::string> httpBlock = getHttpBlock().keyValue;
    for (std::map<std::string, std::string>::iterator it = httpBlock.begin(); it != httpBlock.end(); ++it) {
        std::cout << "    Key: " << it->first << ", Value: " << it->second << std::endl;
        this->handleHTTPBlock();
    }

    // Print server block information
    std::cout << "\n*** Server Blocks ***\n";
    for (std::vector<ServerBlock>::iterator it = getServerBlocks().begin(); it != getServerBlocks().end(); ++it) {
        std::cout << "\n  *** Server Block ***\n";
        std::map<std::string, std::string> serverBlockKV = it->keyValue;
        for (std::map<std::string, std::string>::iterator it2 = serverBlockKV.begin(); it2 != serverBlockKV.end(); ++it2) {
//            std::cout << "      Key: " << it2->first << ", Value: " << it2->second << std::endl;
            this->handleServerBlock(it2->first, it2->second);
        }
        // Print error pages for this server block
        if (!it->errorPages.empty()) {
            std::cout << "      Default error pages:";
            for (std::vector<std::string>::iterator it22 = it->errorPages.begin();
                 it22 != it->errorPages.end(); ++it22) {
                this->handleServErrorPages();
//                std::cout << *it22 << " ";
            }
//            std::cout << std::endl;
        }

        // Print location blocks information within this server block
        for (std::vector<LocationBlock>::iterator it3 = it->locationBlock.begin();
             it3 != it->locationBlock.end(); ++it3) {
            std::cout << "\n    *** Location Block ***\n";
            std::map<std::string, std::string> locationBlockKV = it3->keyValue;
            for (std::map<std::string, std::string>::iterator it4 = locationBlockKV.begin(); it4 != locationBlockKV.end(); ++it4) {
//                std::cout << "        Key: " << it4->first << ", Value: " << it4->second << std::endl;
                this->handleLocationBlock();
            }
            // Print error pages for this location block
            if (!it3->retErrorPages.empty()) {
                std::cout << "        Error Pages: ";
                for (std::vector<std::string>::iterator it5 = it3->retErrorPages.begin(); it5 != it3->retErrorPages.end(); ++it5) {
                    this->handleLocErrorPages();
//                    std::cout << *it5 << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}

