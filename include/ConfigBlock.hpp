#ifndef CONFIGBLOCK_HPP
# define CONFIGBLOCK_HPP

# include "StateMachine.hpp"
# include "Server.hpp"
# include "Location.hpp"
//# include "ConfigParser.hpp"


//class   Server {
//    private:
//        uint16_t						_port;
//        char*                           _ip;
//        std::string						_server_name;
//        std::string						_root;
//        std::string						_index;
//        unsigned long					_client_max_body_size;
//        bool							_autoindex;
//        std::map<short, std::string>	_error_pages;
//
//    public:
//        void                            setPort(uint16_t port) { this->_port = port; }
//        void                            setServerName(std::string server_name) { this->_server_name = server_name; }
//        void                            setAutoIndex(bool autoindex) { this->_autoindex = autoindex; };
//};
//
//class   Location {
//    private:
//        std::string					_path;
//        std::string					_root;
//        bool						_autoindex;
//        std::string					_index;
//        std::vector<short>			_methods; // GET+ POST- DELETE- PUT- HEAD-
//        std::string					_return;
//        std::string					_alias;
//        std::vector<std::string>	_cgi_path;
//        std::vector<std::string>	_cgi_ext;
//        unsigned long				_client_max_body_size;
//};

class ConfigBlock : virtual public StateMachine {
    protected:
        HttpBlock                   _httpBlock;
        std::vector<ServerBlock>    _serverBlocks;
        Server                      _server;
        Location                    _location;

    public:
        ConfigBlock() {}
        ~ConfigBlock() {}

        HttpBlock                   &getHttpBlock() { return this->_httpBlock; }
        std::vector<ServerBlock>&   getServerBlocks() { return this->_serverBlocks; }
        void                        handleBlock();
        void                        handleLocErrorPages();
        void                        handleServErrorPages();
        void                        handleLocationBlock();
        void                        handleServerBlock(std::string key, std::string value);
        void                        handleHTTPBlock();
//        void                        toConfigDTO(Config config) {}
};


#endif
