//
// Created by trimia on 28/05/24.
//
#include "../include/include.h"
#include "../include/Webserver.hpp"

// extern Webserver* ws_ptr;


std::string getMimeType(std::string extension){
    std::map<std::string, std::string> extensionMap;
    extensionMap["txt"]="text/plain";
    extensionMap["html"]="text/html";
    extensionMap["css"]="text/css";
    extensionMap["js"]="text/javascript";
    extensionMap["jpg"]="image/jpeg";
    extensionMap["jpeg"]="image/jpeg";
    extensionMap["png"]="image/png";
    extensionMap["gif"]="image/gif";
    extensionMap["ico"]="image/x-icon";
    extensionMap["bmp"]="image/bmp";
    extensionMap["svg"]="image/svg+xml";
    extensionMap["mp3"]="audio/mpeg";
    extensionMap["mp4"]="video/mp4";
    extensionMap["webm"]="video/webm";
    extensionMap["json"]="application/json";
    extensionMap["xml"]="application/xml";
    extensionMap["pdf"]="application/pdf";
    extensionMap["zip"]="application/zip";
    extensionMap["gzip"]="application/gzip";
    extensionMap["tar"]="application/x-tar";
    extensionMap["7z"]="application/x-7z-compressed";
    extensionMap["rar"]="application/vnd.rar";
    extensionMap["doc"]="application/msword";
    extensionMap["docx"]="application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    extensionMap["xls"]="application/vnd.ms-excel";
    extensionMap["xlsx"]="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    extensionMap["ppt"]="application/vnd.ms-powerpoint";
    extensionMap["pptx"]="application/vnd.openxmlformats-officedocument.presentationml.presentation";
    extensionMap["odt"]="application/vnd.oasis.opendocument.text";
    extensionMap["ods"]="application/vnd.oasis.opendocument.spreadsheet";
    extensionMap["odp"]="application/vnd.oasis.opendocument.presentation";
    extensionMap["csv"]="text/csv";
    extensionMap["tsv"]="text/tab-separated-values";
    extensionMap["rtf"]="application/rtf";
    return extensionMap[extension];
}

std::string getFileExtension(const std::string& filename) {
    // std::string extension;
    // if(filename.find_last_of(".") == std::string::npos)
    //       extension = "txt";
    // else
    printCharsAndSpecialChars(filename);
    std::string extension = filename.substr(filename.find_last_of('.')+1);
    printCharsAndSpecialChars(extension);
    // printCharsAndSpecialChars(extension);
    return extension;
}

std::string StatusString(int statusCode)
{
    switch (statusCode) {
        case 100: return "100 Continue";
        case 101: return "101 Switching Protocols";
        case 200: return "200 OK";
        case 201: return "201 Created";
        case 204: return "204 No Content";
        case 301: return "301 Moved Permanently";
        case 302: return "302 Found";
        case 303: return "303 See Other";
        case 304: return "304 Not Modified";
        case 307: return "307 Temporary Redirect";
        case 308: return "308 Permanent Redirect";
        case 400: return "400 Bad Request";
        case 401: return "401 Unauthorized";
        case 403: return "403 Forbidden";
        case 404: return "404 Not Found";
        case 405: return "405 Method Not Allowed";
        case 406: return "406 Not Acceptable";
        case 408: return "408 Request Timeout";
        case 410: return "410 Gone";
        case 413: return "413 Request Entity Too Large";
        case 500: return "500 Internal Server Error";
        case 501: return "501 Not Implemented";
        case 503: return "503 Service Unavailable";
        case 504: return "504 Gateway Timeout";
        case 505: return "505 HTTP Version Not Supported";
        default: return "501 Not Implemented";
    }
}

bool isDigits(const std::string &str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}
// std::string buildHttpResponseHeader(std::string httpVersion,
//     std::string statusText, std::string& contentType, size_t contentLength) {
//
//     std::ostringstream header;
//     header << httpVersion << " " << statusText << "\r\n";
//     header << "Content-Type: " << contentType << "\r\n";
//     header << "Content-Length: " << contentLength << "\r\n";
//     header << "\r\n";  // End of header
//     // this->_headerSize = header.str().length();
//     return header.str();
// }

bool allowMethod(std::vector<std::string> methods,std::string method)
{
    std::vector<int>allowMethods;
    for (std::vector<std::string>::const_iterator it = methods.begin();it != methods.end(); ++it)
    {
        if(it->compare(method) == 0)
            return true;
        //     allowMethods[GET]=1;
        // else if(it->compare("POST") == 0)
        //     allowMethods[POST]=1;
        // else if(it->compare("DELETE") == 0)
        //     allowMethods[DELETE]=1;
        // else if(it->compare("PUT") == 0)
        //     allowMethods[PUT]=1;
        // else if(it->compare("HEAD") == 0)
        //     allowMethods[HEAD]=1;
        // // else if(it->c_str()=="OPTIONS")
        // 	this->_allowmethods.push_back(OPTIONS);
        // else if(it->c_str()=="TRACE")
        // 	this->_allowmethods.push_back(TRACE);
        // else if(it->c_str()=="CONNECT")
        // 	this->_allowmethods.push_back(CONNECT);
        // else if(it->c_str()=="PATCH")
        // 	this->_allowmethods.push_back(PATCH);
        // else
        // 	this->_allowmethods.push_back(0);

    }

    return false;
}

std::string toStr(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

/**
 * @brief Convert a string to an integer
 * pass it only a string that contains a number
 * @param str
 * @return int
 */

int toInt(std::string str)
{
    // printCharsAndSpecialChars(str);
    int number=0;
    char *p=NULL;
    number=(int)std::strtol(str.c_str(), &p, 10);
    if(*p!='\0')
        throw std::invalid_argument("toInt: invalid number");
    return number;
}

static std::string trim(std::string str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) {
        start++;
    }

    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    return str.substr(start, end - start);
}

static std::vector<std::string> tokenize(std::string s, std::string del)
{
    std::vector<std::string>    tokens;
    size_t start = 0, end = -1 * del.size();

    while ((end = s.find(del, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + del.size();
    }

    if (start < s.length()) {
        tokens.push_back(s.substr(start));
    }

    return tokens;
}

std::vector<std::string> AKAftSplit(std::string& str, std::string div) {
    std::vector<std::string>    tokens;
    std::string                 temp;

    temp = trim(str);
    tokens = tokenize(temp, div);
    return tokens;
}


/**
 *uncomment to debug is no cpp98
 * @brief Convert an integer to a string
 * @param n
 * @return std::string
 */

//#include <iostream>
//#include <iomanip>
//#include <cctype>
//
//void printCharsAndSpecialChars(std::string str) {
//	for (char c : str) {
//		if (std::isprint(c)) {
//			std::cout << c << ' ';
//		} else {
//			std::cout<<CYAN<<"print special char" << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << ' '<<RESET_COLOR<<std::endl;
//		}
//	}
//	std::cout << '\n';
//}

#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>

void printCharsAndSpecialChars(const std::string& str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        char c = *it;
        if (std::isprint(c)) {
            std::cout << c << ' ';
        } else {
            std::cout<<CYAN<<"print special char" << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << ' '<<RESET_COLOR<<std::endl;
        }
    }
    std::cout << '\n';
}