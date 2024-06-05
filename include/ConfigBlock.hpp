#ifndef CONFIGBLOCK_HPP
# define CONFIGBLOCK_HPP

# include "StateMachine.hpp"
# include "Server.hpp"
# include "Location.hpp"


class ConfigBlock : virtual public StateMachine {
protected:
    HttpBlock                   _httpBlock;
    std::vector<ServerBlock>    _serverBlocks;
//    std::vector<Location *>     _listOfLocation; // Add this line

    public:
        ConfigBlock() {}
        ~ConfigBlock() {}

        HttpBlock                   &getHttpBlock() { return this->_httpBlock; }
        std::vector<ServerBlock>&   getServerBlocks() { return this->_serverBlocks; }
        std::vector<Server>         handleBlock();
        void                        handleHTTPBlock();
//    std::vector<Location *> getListOfLocation() const { return _listOfLocation; } // Add a getter for listOfLocation

};

#endif
