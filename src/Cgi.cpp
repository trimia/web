#include "Cgi.hpp"
#include "Webserver.hpp"
#include <string>

/*
 * From std::string to char*
 * */
char*   strToChar(std::string src) {
    uint8_t i = 0;
    uint8_t len = src.length();
    char    out[len + 1];

    while (i <= len) {
        out[i] = src[i];
        i++;
    }
    out[i] = 0;

    return out;
}

/*
 * 
 * CHECK THIS FUNCTION: CAN CHANGE BASED ON Request.cpp
 *
 * This function has to be called in the checkType function
 * First thing first, set the "cgi_enable" variable and the URL
 * As constructor parameters
 *
 * Checks if URL is a valid cgi path
 * Evaluate the correct language extension
 * */
bool    Cgi::checkCgiUrl() {
    this->_isCgiRequest = _pathFile.find("/cgi-bin/") != std::string::npos;

    if (this->_isCgiRequest) // add more language extensions
        this->_isCgiRequest = _pathFile.find(".py") != std::string::npos;

    return this->_isCgiRequest;
}

/*
 * Sanitizing function to get the correct parameters 
 * get the correct _scriptPath
 * CHECK THIS FUNCTION 
 * */
void Cgi::sanitize() {
    uint8_t cgiBinStart = _pathFile.find("/cgi-bin/");

    if (cgiBinStart == std::string::npos || cgiBinStart == 0) {
        this->_scriptPath = "";
        return;
    }

    uint8_t scriptStart = cgiBinStart + strlen("/cgi-bin/");
    while (scriptStart < _pathFile.size() && _pathFile[scriptStart] == '/') {
        scriptStart++;
    }

    uint8_t scriptEnd = scriptStart;
    while (scriptEnd < _pathFile.size() && (std::isalnum(_pathFile[scriptEnd]) || _pathFile[scriptEnd] == '_')) {
        scriptEnd++;
    }

    while (scriptEnd < _pathFile.size() && !std::isalnum(_pathFile[scriptEnd]) && _pathFile[scriptEnd] != '_') {
        scriptEnd++;
    }

    this->_scriptPath = strToChar(_pathFile.substr(scriptStart, scriptEnd - scriptStart));
}


/*
 * EX. HTTP request :
 * GET /search?q=books&sort=newest HTTP/1.1
 * 
 * QUERY_STRING : q=books&sort=newest -> ok
 * REQUEST_METHOD : GET               -> ok
 * REQUEST_URI : /search?q=books&sort=newest
 * 
 * Set environment variables
 * Should I check more conditions?
 * */
void    Cgi::setEnv() {
    this->_envVars.insert(std::make_pair("CONTENT_TYPE", "application/x-www-form-urlencoded"));
    this->_envVars.insert(std::make_pair("CONTENT_LENGTH", toStr(this->_body.size()))); /// ok toStr ? is it c++98 friendly ?
    this->_envVars.insert(std::make_pair("PATH_INFO", std::string(this->_scriptPath))); /// ?
    this->_envVars.insert(std::make_pair("SERVER_PORT", this->_port));
    this->_envVars.insert(std::make_pair("SERVER_SOFTWARE", "Webserve/1.0"));
    this->_envVars.insert(std::make_pair("QUERY_STRING", this->_queryString));
    this->_envVars.insert(std::make_pair("REQUEST_METHOD", this->_method));
    this->_envVars.insert(std::make_pair("REQUEST_URI", this->_requestUri));
    this->_envVars.insert(std::make_pair("SCRIPT_NAME", this->_scriptPath));
    this->_envVars.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
    this->_envVars.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
    this->_envVars.insert(std::make_pair("REDIRECT_STATUS", "200"));
    this->_envVars.insert(std::make_pair("BODY", this->_body));

    this->_envVars.insert(std::make_pair("PATH_TRANSLATED", ""));
    this->_envVars.insert(std::make_pair("REMOTE_IDENT", ""));
    this->_envVars.insert(std::make_pair("REMOTE_ADDR", ""));
    this->_envVars.insert(std::make_pair("REMOTE_HOST", ""));
    this->_envVars.insert(std::make_pair("REMOTE_USER", ""));
    this->_envVars.insert(std::make_pair("SERVER_NAME", ""));
    this->_envVars.insert(std::make_pair("AUTH_TYPE", ""));

    //query map filling

}

/*
 * Core function for the CGI handling
 * Set environment variables
 * Child process and script execution
 * */
#include <fcntl.h>
#include <sys/wait.h>
#include <cstdlib>
void    Cgi::executeCgi() {
    if (this->_isCgiRequest) {

        pid_t pid = fork();
        if (pid == -1) {
            std::cout << "fork" << std::endl;
            exit(2);
        }

        if (pid == 0) {
            this->setEnv();
            this->sanitize();

            // EX: Open a file for writing the script's log (implement this logic)
            int outputFd = open("script_log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (outputFd == -1) {
                std::cout << "open" << std::endl;
                exit(2);
            }

            int dup2Result = dup2(outputFd, STDOUT_FILENO);
            if (dup2Result == -1) {
                std::cout << "dup2" << std::endl;
                close(outputFd);
                exit(2);
            }
            close(outputFd);

            this->_argv[2];
            this->_argv[1] = this->_scriptPath;
            this->_argv[2] = NULL;
            execve(this->_scriptPath, this->_argv, NULL);
        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                return ;
            } else {
                std::cerr << "CGI script exited with error status: " << WEXITSTATUS(status) << std::endl;
                exit(2);
            }
        }
    }
}

/*
 * Use the CGI constructor when filled 
 * the Request parameters from HTTP request
 * then Cgi.executeCgi()  
 */
Cgi::Cgi(Request request) {
    this->_isCgiRequest = request.cgi();
    this->_requestUri = request.request_url();  // is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
    this->_pathFile = request.path_file(); // la url senza query //! /www/html/index.html
    this->_method = request.method();

    ////////////////// getQuery getBody(), _queryMap are missing from Request.hpp (how to get port parameter ?);
    this->_port = ""; /// ?
    this->_queryMap = request._queryMap;
    this->_queryString = request._queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
    this->_body = request.getBody();
    //////////////////
}

Cgi::Cgi(Cgi const &obj)
{
	if (this != &obj)
		*this = obj;
}

Cgi	&Cgi::operator=(const Cgi &obj)
{
	if (this != &obj)
	{
		//	this->attributes = obj.attributes;
		//	...
	}
	return (*this);
}
