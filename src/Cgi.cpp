# include "../include/Cgi.hpp"
#include <fcntl.h>
#include <sys/wait.h>
#include <cstdlib>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>


/*
 * Loop until no more data
 * is available on pipe
 * */
std::string readScriptOutputFromPipe(int pipeFd) {
    std::string output;
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(pipeFd, buffer, sizeof(buffer))) > 0) {
        if (bytesRead > 0) {
            output.append(buffer, bytesRead);
        } else if (bytesRead == -1) {
            std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
            break;
        }
    }

    return output;
}

/*
 * From std::string to char*
 * */
char *strToChar(const std::string &src) {
    char *out = new char[src.length() + 1];
    std::strcpy(out, src.c_str());
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
void Cgi::checkCgiExtension() {
    uint8_t len = _pathFile.length();

    if (len >= 3 && _pathFile.substr(len - 3) == ".py") {
        this->_executable = strToChar("/usr/bin/python3");
    } else if (len >= 3 && _pathFile.substr(len - 3) == ".pl") {
        this->_executable = strToChar("/usr/bin/perl");
    } else if (len >= 3 && _pathFile.substr(len - 3) == ".sh") {
        this->_executable = strToChar("/usr/bin/bash");
    } else {
        this->_executable = strToChar("NOT IMPLEMENTED");
    }
}

/*
 * Sanitizing function to get the correct parameters
 * get the correct _scriptPath
 * CHECK THIS FUNCTION
 * */
//void Cgi::sanitize() {
//	uint8_t cgiBinStart = _pathFile.find("/cgi-bin/");
//
//	if (cgiBinStart == std::string::npos || cgiBinStart == 0) {
//		return;
//	}
//
//	uint8_t scriptStart = cgiBinStart + strlen("/cgi-bin/");
//	while (scriptStart < _pathFile.size() && _pathFile[scriptStart] == '/') {
//		scriptStart++;
//	}
//
//	uint8_t scriptEnd = scriptStart;
//	while (scriptEnd < _pathFile.size() && (std::isalnum(_pathFile[scriptEnd]) || _pathFile[scriptEnd] == '_')) {
//		scriptEnd++;
//	}
//
//	while (scriptEnd < _pathFile.size() && !std::isalnum(_pathFile[scriptEnd]) && _pathFile[scriptEnd] != '_') {
//		scriptEnd++;
//	}
//
//	this->_scriptPath = strToChar(_pathFile.substr(scriptStart, scriptEnd - scriptStart));
//}

char **Cgi::getEnv() {
    char **env = new char *[this->_envVars.size() + 1];
    int c = 0;

    for (std::map<std::string, std::string>::const_iterator it = this->_envVars.begin();
         it != this->_envVars.end(); it++) {
        env[c++] = strToChar(it->first + "=" + it->second);
    }
    env[c] = NULL;

    return env;
}

void Cgi::freeEnv(char **environment) {
    for (size_t i = 0; environment[i]; i++)
        delete[] environment[i];
    delete[] environment;
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
void Cgi::setEnv() {

    this->checkCgiExtension();
    this->_envVars.insert(std::make_pair("CONTENT_TYPE", "application/x-www-form-urlencoded"));
    this->_envVars.insert(std::make_pair("CONTENT_LENGTH", toStr(this->_body.size()))); /// ok toStr ? is it c++98 friendly ?
    this->_envVars.insert(std::make_pair("PATH_INFO", this->_pathFile)); /// ?
    this->_envVars.insert(std::make_pair("SERVER_PORT", this->_port));
    this->_envVars.insert(std::make_pair("SERVER_SOFTWARE", "Webserve/1.0"));
    this->_envVars.insert(std::make_pair("QUERY_STRING", this->_queryString));
    this->_envVars.insert(std::make_pair("REQUEST_METHOD", this->_method));
    this->_envVars.insert(std::make_pair("REQUEST_URI", this->_requestUri));
    this->_envVars.insert(std::make_pair("SCRIPT_NAME", this->_executable));
    this->_envVars.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
    this->_envVars.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
    this->_envVars.insert(std::make_pair("REDIRECT_STATUS", "200"));
    this->_envVars.insert(std::make_pair("BODY", this->_body));
    printf("envVars:::::::::::::::::::: |%s|\n\n", this->_envVars["BODY"].c_str());

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

std::string Cgi::executeCgi() {
    std::string scriptOutput;
    this->setEnv();
    char **envp = this->getEnv();

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        std::cerr << "CGI: Error creating pipe for communication" << std::endl;
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "CGI: Error creating child process" << std::endl;
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
    }

    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        this->_argv[0] = this->_executable;
        this->_argv[1] = strToChar("./cgi-bin/script.py");
        this->_argv[2] = NULL;

        if (execve(this->_executable, this->_argv, envp) == -1) {
            std::cerr << "CGI: Error executing script: " << strerror(errno) << std::endl;
            this->freeEnv(envp);
            exit(EXIT_FAILURE);
        }
    } else {
        close(pipe_fd[1]);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            scriptOutput = readScriptOutputFromPipe(pipe_fd[0]);
        } else {
            std::cerr << "CGI: Script exited with error status: " << WEXITSTATUS(status) << std::endl;
        }

        close(pipe_fd[0]);
    }

    this->freeEnv(envp);
    std::cout << RED << scriptOutput << RESET_COLOR << std::endl;
    return scriptOutput;
}


/*
 * Use the CGI constructor when filled
 * the Request parameters from HTTP request
 * then Cgi.executeCgi()
 */
Cgi::Cgi(Request *request) { ////// SET CLIENT INSTEAD OF REQUEST TO HAVE CLIENT.REQUEST
//	this->_isCgiRequest = request->cgi();
    this->_requestUri = request->request_url();  // is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
    this->_pathFile = request->path_file(); // la url senza query //! /www/html/index.html
    this->_method = request->method();

    printf("CGI CONSTRUCTOR: |%s|%s|\n\n\n", _requestUri.c_str(), _pathFile.c_str());

    //////	check values from the request to client object and workflow logic
    ////	create a response from the executeCgi() return string(html file)
    //		insert call functions inside Webserver::_handleConnection


    ////////////////// getQuery getBody(), _queryMap are missing from Request.hpp (how to get port parameter ?);
    // this->_port = ""; /// ?
    // this->_queryMap = request._queryMap;
    // this->_queryString = request._queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
    // this->_body = request.getBody();
    //////////////////
}

Cgi::Cgi(Cgi const &obj) {
    if (this != &obj)
        *this = obj;
}

Cgi &Cgi::operator=(const Cgi &obj) {
    if (this != &obj) {
        //	this->attributes = obj.attributes;
        //	...
    }
    return (*this);
}
