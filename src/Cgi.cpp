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
    for(std::map<std::string, std::string>::iterator it = this->_queryMap.begin(); it != this->_queryMap.end(); ++it) {
        std::string key = it->first;
        std::string value = it->second;
        this->_envVars.insert(std::make_pair(key, value));
    }
    this->_envVars.insert(std::make_pair("REQUEST_METHOD", this->_method));
    this->_envVars.insert(std::make_pair("REQUEST_URI", this->_requestUri));
    this->_envVars.insert(std::make_pair("SCRIPT_NAME", this->_executable));
    this->_envVars.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
    this->_envVars.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
    this->_envVars.insert(std::make_pair("REDIRECT_STATUS", "200"));
    this->_envVars.insert(std::make_pair("BODY", this->_body));
    this->_envVars.insert(std::make_pair("PATH_TRANSLATED", ""));
    this->_envVars.insert(std::make_pair("REMOTE_IDENT", ""));
    this->_envVars.insert(std::make_pair("REMOTE_ADDR", ""));
    this->_envVars.insert(std::make_pair("REMOTE_HOST", ""));
    this->_envVars.insert(std::make_pair("REMOTE_USER", ""));
    this->_envVars.insert(std::make_pair("SERVER_NAME", ""));
    this->_envVars.insert(std::make_pair("AUTH_TYPE", ""));
}

/*
 * Core function for the CGI handling
 * Set environment variables
 * Child process and script execution
 * */

void Cgi::executeCgi(Client *client) {
    std::string scriptOutput;
    this->setEnv();
    char **envp = this->getEnv();
    struct stat infoFile;
    stat(this->_pathFile.c_str(), &infoFile);
    if (infoFile.st_size < 0) {
        std::cout << RED << "Error stat()" << RESET_COLOR << std::endl;
        client->response()->set_status_code(404);
        return;
    }

    std::ifstream file(this->_pathFile.c_str(), std::ios::binary);
    // Check if the file was opened successfully
    if (!file.is_open()){
        std::cout << RED << "Error file open" << RESET_COLOR << std::endl;
        client->response()->set_status_code(404);
        return;
    }


    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        std::cout<<RED << "Error CGI creating pipe for communication" <<RESET_COLOR <<std::endl;
        client->response()->set_status_code(500);
        return ;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cout<<RED << "Error CGI creating child process" << RESET_COLOR<<std::endl;
        client->response()->set_status_code(500);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return ;
    }

    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        this->_argv[0] = this->_executable;
        this->_argv[1] = strToChar(this->_pathFile);
        this->_argv[2] = NULL;

        if (execve(this->_executable, this->_argv, envp) == -1) {
            std::cout<<RED << "Error CGI executing script: " <<RESET_COLOR << std::endl;
            client->response()->set_status_code(500);
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
            std::cerr << "Error CGI Script exited with error status: " << WEXITSTATUS(status) << std::endl;
            client->response()->set_status_code(500);
        }

        close(pipe_fd[0]);
    }

    this->freeEnv(envp);
    std::cout << RED << scriptOutput << RESET_COLOR << std::endl;
    client->response()->set_body(scriptOutput);
    client->response()->set_body_size(scriptOutput.length());
    client->response()->set_file_extension("html");
    client->response()->set_status_code(200);
}
/*
 * Use the CGI constructor when filled
 * the Request parameters from HTTP request
 * then Cgi.executeCgi()
 */
Cgi::Cgi(Request *request) { ////// SET CLIENT INSTEAD OF REQUEST TO HAVE CLIENT.REQUEST
    this->_requestUri = request->request_url();
    if(request->path_file().find("cgi-bin/script.py")!=std::string::npos)// is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
        this->_pathFile = "./cgi-bin/script.py"; // la url senza query //! /www/html/index.html
    if(request->path_file().find("cgi-bin/getQuery.py")!=std::string::npos)// is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
        this->_pathFile = "./cgi-bin/getQuery.py"; // la url senza query //! /www/html/index.html
    this->_method = request->method();
    this->_queryMap = request->getQueryMap();
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
