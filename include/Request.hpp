#ifndef _Request_HPP_
#define _Request_HPP_

#include "Client.hpp"
#include "include.h"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //

class Client;

class	Request
{
	public	:
		Request ();
		Request (Request const &obj);
		~Request ();
		Request &operator= (const Request &obj);

        void receiveData(Client *client);

        int parseRequest();
		void fillRequest();
		std::string checktype(std::string input);

		bool error() const;

		void set_error(bool error);

		bool cgi() const;

		void set_cgi(bool cgi);

		bool ended() const;

		void set_ended(bool ended);

		bool complete() const;

		void set_complete(bool complete);

		std::time_t time_start() const;

		void set_time_start(std::time_t time_start);

		void setRequestHeaders(const std::map<std::string, std::string> &requestHeaders);

		size_t body_size() const;

		void set_body_size(size_t body_size);

		std::map<std::string, std::string> request_headers() const;

		void set_request_headers(const std::map<std::string, std::string> &request_headers);

		std::string http_message();

		void set_http_message(const std::string &http_message);

		std::string method() const;

		void set_method(const std::string &method);

		size_t header_size();

		void set_header_size(size_t header_size);

	private	:

        bool									_error;
        bool									_cgi;
        bool									_ended;
		bool									_complete; // finito di leggere o serve un altro giro
		std::time_t								_timeStart;
		size_t									_body_size;
		std::map<std::string, std::string>		_requestHeaders;
		std::string								_method; // GET, POST, DELETE //! GET
		size_t									_headerSize;
		std::string								_httpMessage; // messaggio intero preso a pezzi


		bool			_hasBeenClosed;//?? necessary here or in client
		bool									_parsed;
		bool									_answered;
		std::string								_requestURL; // tutta la url //! /www/html/index.html?ciao=asd/bella=zi
		std::string								_path_file; // la url senza query //! /www/html/index.html
		std::vector<std::string>				_folders; // le cartelle del URL //! www html
		std::string								_fileName; // solo il file nella URL //! index.html
		std::string								_extension; // estensione del file se esiste //! html
		std::string								_queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
		std::map<std::string, std::string>		_queryMap; // querry divisa per: chiave=valore&... //! ciao[asd] bella[zi]
		std::string								_version; // HTTP1.1

		std::string								_path;
		//        //maybe insted of string string we can do httmethod string or maybe isn't necessary a map
		std::map<std::string, std::string>  _request_headers;
		//        HttpMethod                          _method;
		std::string                         _body;
		size_t                              _body_length;
        short                               _error_code;
        std::string                         _query;//?
        //    std::vector<u_int8_t>               _body;

    //	DataType	attributes.
};



// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
