// # include "../include/Cgi.hpp"
// #include <fcntl.h>
// #include <sys/wait.h>
// #include <cstdlib>
//
// #include <fcntl.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <iostream>
// #include <string>
// #include <fstream> // For file I/O
//
// /*
//  * From std::string to char*
//  * */
//  char*   strToChar(std::string src) {
//      uint8_t i = 0;
//      uint8_t len = src.length();
//      char    out[len + 1];
//
//      while (i <= len) {
//          out[i] = src[i];
//          i++;
//      }
//      out[i] = 0;
//
//      return out;
//  }
//
//  /*
//   *
//   * CHECK THIS FUNCTION: CAN CHANGE BASED ON Request.cpp
//   *
//   * This function has to be called in the checkType function
//   * First thing first, set the "cgi_enable" variable and the URL
//   * As constructor parameters
//   *
//   * Checks if URL is a valid cgi path
//   * Evaluate the correct language extension
//   * */
//  bool    Cgi::checkCgiUrl() {
//      this->_isCgiRequest = _pathFile.find("/cgi-bin/") != std::string::npos;
//
//      if (this->_isCgiRequest) // add more language extensions
//          this->_isCgiRequest = _pathFile.find(".py") != std::string::npos;
//
//      return this->_isCgiRequest;
//  }
//
//  /*
//   * Sanitizing function to get the correct parameters
//   * get the correct _scriptPath
//   * CHECK THIS FUNCTION
//   * */
//  void Cgi::sanitize() {
//      uint8_t cgiBinStart = _pathFile.find("/cgi-bin/");
//
//      if (cgiBinStart == std::string::npos || cgiBinStart == 0) {
//          this->_scriptPath = "";
//          return;
//      }
//
//      uint8_t scriptStart = cgiBinStart + strlen("/cgi-bin/");
//      while (scriptStart < _pathFile.size() && _pathFile[scriptStart] == '/') {
//          scriptStart++;
//      }
//
//      uint8_t scriptEnd = scriptStart;
//      while (scriptEnd < _pathFile.size() && (std::isalnum(_pathFile[scriptEnd]) || _pathFile[scriptEnd] == '_')) {
//          scriptEnd++;
//      }
//
//      while (scriptEnd < _pathFile.size() && !std::isalnum(_pathFile[scriptEnd]) && _pathFile[scriptEnd] != '_') {
//          scriptEnd++;
//      }
//
//      this->_scriptPath = strToChar(_pathFile.substr(scriptStart, scriptEnd - scriptStart));
//  }
//
// char**	Cgi::getEnv() {
//     char	**env = new char*[this->_envVars.size() + 1];
//     int		c = 0;
//
//     for (std::map<std::string, std::string>::const_iterator it = this->_envVars.begin(); it != this->_envVars.end(); it++) {
//         env[c++] = strToChar(it->first + "=" + it->second);
//     }
//     env[c] = NULL;
//
//     return env;
// }
//
// void    Cgi::freeEnv(char** environment) {
//     for (size_t i = 0; environment[i]; i++)
//         delete[] environment[i];
//     delete[] environment;
//  }
//
//  /*
//   * EX. HTTP request :
//   * GET /search?q=books&sort=newest HTTP/1.1
//   *
//   * QUERY_STRING : q=books&sort=newest -> ok
//   * REQUEST_METHOD : GET               -> ok
//   * REQUEST_URI : /search?q=books&sort=newest
//   *
//   * Set environment variables
//   * Should I check more conditions?
//   * */
//  void    Cgi::setEnv() {
//      this->_envVars.insert(std::make_pair("CONTENT_TYPE", "application/x-www-form-urlencoded"));
//     //  this->_envVars.insert(std::make_pair("CONTENT_LENGTH", toStr(this->_body.size()))); /// ok toStr ? is it c++98 friendly ?
//      this->_envVars.insert(std::make_pair("PATH_INFO", std::string(this->_scriptPath))); /// ?
//      this->_envVars.insert(std::make_pair("SERVER_PORT", this->_port));
//      this->_envVars.insert(std::make_pair("SERVER_SOFTWARE", "Webserve/1.0"));
//      this->_envVars.insert(std::make_pair("QUERY_STRING", this->_queryString));
//      this->_envVars.insert(std::make_pair("REQUEST_METHOD", this->_method));
//      this->_envVars.insert(std::make_pair("REQUEST_URI", this->_requestUri));
//      this->_envVars.insert(std::make_pair("SCRIPT_NAME", this->_scriptPath));
//      this->_envVars.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
//      this->_envVars.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
//      this->_envVars.insert(std::make_pair("REDIRECT_STATUS", "200"));
//      this->_envVars.insert(std::make_pair("BODY", this->_body));
//
//      this->_envVars.insert(std::make_pair("PATH_TRANSLATED", ""));
//      this->_envVars.insert(std::make_pair("REMOTE_IDENT", ""));
//      this->_envVars.insert(std::make_pair("REMOTE_ADDR", ""));
//      this->_envVars.insert(std::make_pair("REMOTE_HOST", ""));
//      this->_envVars.insert(std::make_pair("REMOTE_USER", ""));
//      this->_envVars.insert(std::make_pair("SERVER_NAME", ""));
//      this->_envVars.insert(std::make_pair("AUTH_TYPE", ""));
//
//      //query map filling
//
//  }
//
//  /*
//   * Core function for the CGI handling
//   * Set environment variables
//   * Child process and script execution
//   * */
// std::string Cgi::executeCgi() {
//     std::string scriptOutput = "";
//
//     if (this->_isCgiRequest) {
//         int pipe_fd[2];
//
//         // Create a temporary file for script output
//         int outputFd = open("script_output.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (outputFd == -1) {
//             std::cerr << "Error creating temporary file for script output" << std::endl;
//             exit(2);
//         }
//
//         if (pipe(pipe_fd) == -1) {
//             std::cerr << "Error creating pipe for communication" << std::endl;
//             close(outputFd);
//             exit(2);
//         }
//
//         pid_t pid = fork();
//         if (pid == -1) {
//             std::cerr << "Error creating child process" << std::endl;
//             close(outputFd);
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//             exit(2);
//         }
//
//         if (pid == 0) {
//             close(pipe_fd[1]);
//
//             dup2(pipe_fd[0], STDOUT_FILENO);
//             close(pipe_fd[0]);
//
//             this->setEnv();
//             char** envp = this->getEnv();
//             this->sanitize();
//
//             execve(this->_scriptPath, this->_argv, envp);
//             this->freeEnv(envp);
//         } else {
//             close(pipe_fd[0]);
//
//             int status;
//             waitpid(pid, &status, 0);
//
//             if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
//                 std::ifstream outputFile("script_output.tmp");
//                 if (outputFile.is_open()) {
//                     std::stringstream ss;
//                     ss << outputFile.rdbuf();
//                     scriptOutput = ss.str();
//                     outputFile.close();
//                 } else {
//                     std::cerr << "Error reading script output from temporary file" << std::endl;
//                 }
//             } else {
//                 std::cerr << "CGI script exited with error status: " << WEXITSTATUS(status) << std::endl;
//             }
//
//             close(outputFd);
//
//         }
//     }
//     /*
//      * TODO setStatusCode
//      * */
//      return scriptOutput;
// }
//
// /*
//  * Use the CGI constructor when filled
//  * the Request parameters from HTTP request
//  * then Cgi.executeCgi()
//  */
// Cgi::Cgi(Request *request) { ////// SET CLIENT INSTEAD OF REQUEST TO HAVE CLIENT.REQUEST
//     this->_isCgiRequest = request->cgi();
//     this->_requestUri = request->request_url();  // is the host tutta la url //! /www/html/index.html?ciao=asd/bella=zi
//     this->_pathFile = request->path_file(); // la url senza query //! /www/html/index.html
//     this->_method = request->method();
//
//     printf("CGI CONSTUCRTOR: %s\n\n\n", _requestUri.c_str());
//     ////////////////// getQuery getBody(), _queryMap are missing from Request.hpp (how to get port parameter ?);
//     // this->_port = ""; /// ?
//     // this->_queryMap = request._queryMap;
//     // this->_queryString = request._queryName; // le query, dopo il '?' nella URL //! ciao=asd/bella=zi
//     // this->_body = request.getBody();
//     //////////////////
// }
//
// Cgi::Cgi(Cgi const &obj)
// {
// 	if (this != &obj)
// 		*this = obj;
// }
//
// Cgi	&Cgi::operator=(const Cgi &obj)
// {
// 	if (this != &obj)
// 	{
// 		//	this->attributes = obj.attributes;
// 		//	...
// 	}
// 	return (*this);
// }
