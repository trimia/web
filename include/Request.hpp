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

		void checkTypeAndSize(std::string input);

		void fillRequest(std::string &httpRequest);

		void setUrlPathQuery(std::string &url);

		std::string getQueryFromHttpRequest(std::string &httpRequest);

		int parseRequest(std::string httpRequest);

        void buildBody(Client *client,char input[RCV_BUF_SIZE],int size);

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

		std::string http_version() const;

		void set_http_version(const std::string &http_version);

		bool is_body() const;

		void set_is_body(bool is_body);

		bool is_query() const;

		void set_is_query(bool is_query);

		std::string request_url() const;

		void set_request_url(const std::string &request_url);

		std::string path_file() const;

		void set_path_file(const std::string &path_file);

		bool is_root() const;

		void set_is_root(bool is_rooth);

    size_t getClientMaxBodySize() const;

    void setClientMaxBodySize(size_t clientMaxBodySize);

    std::string connection() const;

		void set_connection(std::string connection);

		size_t header_size();

		void set_header_size(size_t header_size);

		std::string & file_name();

		void set_file_name(std::string file_name);

		std::string & getExtension();

		void set_extension(std::string extension);

		std::string & getMyme_type();

		void set_myme_type(std::string myme_type);

		bool & getNot_complete();

		void set_not_complete(bool not_complete);

		const std::string &getContentType() const;

    void setContentType(const std::string &contentType);

private	:

        bool									    _error;
        bool									    _cgi;
        bool									    _ended;
		// bool									    _not_complete; // finito di leggere o serve un altro giro
		std::string								    _connection;//keep-alive or close
		std::time_t								    _timeStart;
		bool									    _isBody;
		std::string								    _body;
		size_t									    _body_size;
        std::string								    _content_type;
        size_t                                      _clientMaxBodySize;
		std::string								    _header; // messaggio intero preso a pezzi
		size_t									    _headerSize;
		std::map<std::string, std::string>		    _requestHeaders;
		std::string								    _method; // GET, POST, DELETE //! GET
		std::string								    _requestURL; // is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
		bool									    _isRoot;
		std::string								    _path_file; // la url senza query //! /www/html/index.html
		std::string								    _query;//?
		bool									    _isQuery;//?
		std::string								    _HTTPVersion; // HTTP/1.1 //! HTTP/1.1
        std::string								    _fileName; // if post from our site name of file to upload
		std::string									_extension; // estensione del file se esiste //! html
		std::string									_mymeType; // tipo del file se esiste //! text/html


		bool			_hasBeenClosed;//?? necessary here or in client
		bool									_parsed;
		bool									_answered;
		std::vector<std::string>				_folders; // le cartelle del URL //! www html
		std::string								_queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
		std::map<std::string, std::string>		_queryMap; // querry divisa per: chiave=valore&... //! ciao[asd] bella[zi]

		std::string								_path;
		//        //maybe insted of string string we can do httmethod string or maybe isn't necessary a map
		//        HttpMethod                          _method;
		size_t                              _body_length;
		short                               _error_code;
		//    std::vector<u_int8_t>               _body;

    //	DataType	attributes.
};



// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
