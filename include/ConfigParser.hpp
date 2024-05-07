#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

//# include <iostream>
//# include <string>
//# include <cstring>
//# include <fstream>
//# include <vector>
//# include <map>
//# include <cstdlib>
//# include <stdint.h>
# include "StateMachine.hpp"
# include "ConfigBlock.hpp"

# define IN 0
# define OUT 1
# define ERR 2

class ConfigParser : virtual public StateMachine {
    private:
        std::string                         _parsed_config;
        std::string                         _config_file;
        ConfigBlock                         _configBlock;
    
    public:
        void                                parseConfigFile();
        void                                extractKeyword();
        void                                stateCheck(std::string line);
        void                                handleLine(std::string line);
        void                                handleHttpState(std::string line);
        void                                handleServerState(std::string line);
        void                                handleLocationState(std::string line);

        //////// DEBUG PURPOSE
        void                                printConfig();

        ConfigBlock&                        getConfigBlock() { return this->_configBlock ; }
        ConfigParser(std::string config_file, ConfigBlock& configBlock) {
            this->_config_file = config_file;
            this->_configBlock = configBlock;
            this->insideServerBlock = false;
            this->countServBlocks = 0;
            this->countLocBlocks = 0;
        } ;

        ~ConfigParser() {} ;
};

/*

KEYWORDS TO HANDLE

http
    server
        host localhost
        listen
        server_name
        root /path

        error_page 400-500 error/400-500.html

        body_size

        location /folder_name
            autoindex on
            method POST
            body_size 500M
            return 308 http://127.0.0.1:8080/test_notauthorized/

        index /folder_name-filename.html

        autoindex

        cgi_enable

    server
*/

#endif