#ifndef _Request_HPP_
#define _Request_HPP_

#include "include.h"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //

enum HttpMethod
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

class	Request
{
	public	:
		Request ();
		Request (Request const &obj);
		~Request ();
		Request &operator= (const Request &obj);
        int parseRequest(std::string input, Request httpRequest);

    void setRequestHeaders(const std::map<std::string, std::string> &requestHeaders);

    private	:

        bool									_parsed;
        bool									_is_cgi;
        std::string								_httpMessage; // messaggio intero preso a pezzi
        bool									_complete; // finito di leggere o serve un altro giro
        bool									_answered;
        std::string								_method; // GET, POST, DELETE //! GET
        std::string								_requestURL; // tutta la url //! /www/html/index.html?ciao=asd/bella=zi
        std::string								_path_file; // la url senza query //! /www/html/index.html
        std::vector<std::string>				_folders; // le cartelle del URL //! www html
        std::string								_fileName; // solo il file nella URL //! index.html
        std::string								_extension; // estensione del file se esiste //! html
        std::string								_queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
        std::map<std::string, std::string>		_queryMap; // querry divisa per: chiave=valore&... //! ciao[asd] bella[zi]
        std::string								_version; // HTTP1.1
        std::map<std::string, std::string>		_requestHeaders;

        std::string                         _path;
//        //maybe insted of string string we can do httmethod string or maybe isn't necessary a map
        std::map<std::string, std::string>  _request_headers;
//        HttpMethod                          _method;
        std::string                         _body;
        size_t                              _max_body_size;
        size_t                              _body_length;
        short                               _error_code;
        std::string                         _query;//?
        //    std::vector<u_int8_t>               _body;

//servono??
//    size_t                              _chunk_length;
//    std::map<u_int8_t, std::string>     _method_str;
//    ParsingState                        _state;
//    std::string                         _fragment;
//    std::string                         _storage;
//    std::string                         _key_storage;
//    short                               _method_index;
//    u_int8_t                            _ver_major;
//    u_int8_t                            _ver_minor;
//    std::string                         _server_name;
//    std::string                         _body_str;
    /* flags */
//    bool                                _fields_done_flag;
//    bool                                _body_flag;
//    bool                                _body_done_flag;
//    bool                                _complete_flag;
//    bool                                _chunked_flag;
//    bool                                _multiform_flag;
    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
