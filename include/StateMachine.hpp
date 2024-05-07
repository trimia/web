#ifndef STATEMACHINE_HPP
# define STATEMACHINE_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <fstream>
# include <vector>
# include <map>
# include <cstdlib>
# include <stdint.h>

/*
 * Finite State Machine (FSM)
 * Parent class that provides a defined number of steps
 * that can leverage multiple switch cases
 * so to enable the action needed
 * */

enum BlockState {
    HttpState,
    ServerState,
    LocationState,
    End
};

class LocationBlock {
    public:
        std::map<std::string, std::string>  keyValue;
        std::vector<std::string>            retErrorPages;
        std::string                         pathDir;
        LocationBlock() {}
        ~LocationBlock() {}
};

class ServerBlock {
    public:
        std::vector<LocationBlock>          locationBlock;
        std::map<std::string, std::string>  keyValue;
        std::vector<std::string>            errorPages;

        ServerBlock() {}
        ~ServerBlock() {}
};

class HttpBlock {
    public:
        std::map<std::string, std::string>  keyValue;
        std::vector<std::string>            errorPages;
        HttpBlock() {}
        ~HttpBlock() {}
};

class StateMachine {
    protected:
        bool            insideServerBlock;
        int             countServBlocks;
        int             countLocBlocks;
        int             currentState;
        std::string     currentKey;
        std::string     currentValue;

};

#endif
