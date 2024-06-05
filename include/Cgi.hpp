#ifndef _Cgi_HPP_
# define _Cgi_HPP_

//# include <iostream>
//# include <map>
//#include <string>
# include "include.h"
# include "Request.hpp"

/*
 * NOT SO OFF-TOPIC :
 * Request-URI is a Uniform Resource Identifier
 * 
 * CGI Program Execution Flow:
 * 
 * Web Server receives request: 
 * Webserver receives an HTTP request from the client.
 * Request contains the information (URL), headers, form data
 *
 * Identify CGI request:
 * Webserver checks if requested URL poitns to a CGI script location
 * Check for a specific directory prefix (/cgi-bin/) in the URL path
 * 
 * Create child process:
 * Webserver forks a child process to isolate the execution of CGI script
 * This ensures the stability of the webserver
 * in case script crashes or encounters errors
 *
 * Set Env variables:
 * Webserver sets env variables for child process based 
 * on the received HTTP request
 * Variables can be:
 * QUERY_STRING: contains form data submitted along with the URL after ? symbol (GET)
 * CONTENT_LENGTH: size of the request body data (POST requests with form data)
 * CONTENT_TYPE: content type of the request body data (application/x-www-form-urlencoded) 
 * REMOTE_ADDR: client IP address
 * REQUEST_METHOD: GET, POST
 *
 * Execute CGI script:
 * Execute CGI script by using execve whitin the child process
 *
 * Capture Script response:
 * Webserver waits for the child process to finish execution.
 * It captures the script response from its STDOUT 
 *
 * Send response to client:
 * Webserver integrates script's response
 * into the final HTTP response sent back to the client
 * */


class	Cgi {
    private	:
        // std::string                         _requestUrl;
        bool                                _isCgiRequest;
        std::string                         _port;
        std::string                         _requestUri;
        std::string                         _pathFile;
        std::string                         _body;
        std::string                         _method;
        std::string                         _queryString;
        std::map<std::string, std::string>  _queryMap;
        std::map<std::string, std::string>  _envVars;
        char**                              _argv;
        char*                               _urlPathSanitized;
        char*                               _scriptPath;

    public	:
        Cgi() {;};
        Cgi(Request *request);
        Cgi(Cgi const &obj);
        ~Cgi() {} ;
        Cgi     &operator=(const Cgi &obj);

        bool    checkCgiUrl();
        void    executeCgi();
        void    setEnv();
        void    sanitize();
};

#endif
