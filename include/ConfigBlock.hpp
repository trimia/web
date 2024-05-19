#ifndef CONFIGBLOCK_HPP
# define CONFIGBLOCK_HPP

# include "StateMachine.hpp"
# include "Server.hpp"
# include "Location.hpp"


class ConfigBlock : virtual public StateMachine {
    protected:
        HttpBlock                   _httpBlock;
        std::vector<ServerBlock>    _serverBlocks;

    public:
        ConfigBlock() {}
        ~ConfigBlock() {}

        HttpBlock                   &getHttpBlock() { return this->_httpBlock; }
        std::vector<ServerBlock>&   getServerBlocks() { return this->_serverBlocks; }
        std::vector<Server>         handleBlock();
        // void                        handleLocVectors(Location location, std::vector<std::string> vecString);
        // void                        handleServVectors(Server server, std::vector<std::string> vecString);
//        void                        handleServerBlock(Server server, std::string key, std::string value);
        void                        handleHTTPBlock();
};

Location    handleLocationBlock(std::map<std::string, std::string> keyValue, std::vector<std::string> methods, std::vector<std::string> errPage);
Server      handleServerBlock(std::map<std::string, std::string> keyValue, std::vector<std::string> vector);


#endif
