//
// Created by test on 6/9/24.
//

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <fstream>
# include <vector>
# include <map>
# include <cstdlib>
# include <stdint.h>
# include "Webserver.hpp"

enum MachineState {
    HttpState,
    ServerState,
    LocationState,
    End
};

class ConfigParser {
private:
    std::string                         _parsedConfig;
    std::string                         _configFile;
    std::vector<Server>                 _vectorOfServers;
    std::vector<Location*>               _vectorOfLocations;

public:
    bool            insideServerBlock;
    int             countServerBlocks;
    int             countLocationBlocks;
    int             currentState;
    std::string     currentKey;
    std::string     currentValue;

    //////// DEBUG PURPOSE
    void                                printConfig();
    std::vector<Server>                 parseConfigFile();
    void                                stateCheck(std::string line);
    void                                handleLine(std::string line);
    void                                handleHttpState(std::string line);
    void                                handleServerState(std::string line);
    void                                handleLocationState(std::string line);



    ConfigParser(std::string configFile) {
        this->_configFile = configFile;
        this->insideServerBlock = false;
        this->countServerBlocks = 0;
        this->countLocationBlocks = 0;
        this->currentState = -1;
        this->_vectorOfServers.reserve(0);
        this->_vectorOfLocations.reserve(0);
    } ;

    ~ConfigParser() {} ;
};


#endif
