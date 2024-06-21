//
// Created by trimia on 28/05/24.
//
#include "../include/include.h"
#include "../include/Webserver.hpp"

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

std::string getExtensionFromMimeType(std::string mimeType){
    std::map<std::string, std::string> mimeTypeMap;
    mimeTypeMap["text/plain"] = "txt";
    mimeTypeMap["text/html"] = "html";
    mimeTypeMap["text/css"] = "css";
    mimeTypeMap["text/javascript"] = "js";
    mimeTypeMap["image/jpeg"] = "jpg";
    mimeTypeMap["image/png"] = "png";
    mimeTypeMap["image/gif"] = "gif";
    mimeTypeMap["image/x-icon"] = "ico";
    mimeTypeMap["image/bmp"] = "bmp";
    mimeTypeMap["image/svg+xml"] = "svg";
    mimeTypeMap["audio/mpeg"] = "mp3";
    mimeTypeMap["video/mp4"] = "mp4";
    mimeTypeMap["video/webm"] = "webm";
    mimeTypeMap["application/json"] = "json";
    mimeTypeMap["application/xml"] = "xml";
    mimeTypeMap["application/pdf"] = "pdf";
    mimeTypeMap["application/zip"] = "zip";
    mimeTypeMap["application/gzip"] = "gzip";
    mimeTypeMap["application/x-tar"] = "tar";
    mimeTypeMap["application/x-7z-compressed"] = "7z";
    mimeTypeMap["application/vnd.rar"] = "rar";
    mimeTypeMap["application/msword"] = "doc";
    mimeTypeMap["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = "docx";
    mimeTypeMap["application/vnd.ms-excel"] = "xls";
    mimeTypeMap["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = "xlsx";
    mimeTypeMap["application/vnd.ms-powerpoint"] = "ppt";
    mimeTypeMap["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = "pptx";
    mimeTypeMap["application/vnd.oasis.opendocument.text"] = "odt";
    mimeTypeMap["application/vnd.oasis.opendocument.spreadsheet"] = "ods";
    mimeTypeMap["application/vnd.oasis.opendocument.presentation"] = "odp";
    mimeTypeMap["text/csv"] = "csv";
    mimeTypeMap["text/tab-separated-values"] = "tsv";
    mimeTypeMap["application/rtf"] = "rtf";

    return mimeTypeMap[mimeType];
}

std::string getFileExtension(const std::string& filename) {
//    printCharsAndSpecialChars(filename);
    std::string extension = filename.substr(filename.find_last_of('.')+1);
//    printCharsAndSpecialChars(extension);
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
        case 409: return "409 Conflict";//when upload in a non existing directory
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

std::string getErrorPages(int statusCode)
{
    switch (statusCode) {

        case 400: return "./errors/400.html";
        case 401: return "./errors/401.html";
        case 403: return "./errors/403.html";
        case 404: return "./errors/404.html";
        case 405: return "./errors/405.html";
        case 409: return "./errors/409.html";
        case 406: return "./errors/406.html";
        case 408: return "./errors/408.html";
        case 410: return "./errors/410.html";
        case 413: return "./errors/413.html";
        case 418: return "./errors/418.html";
        case 500: return "./errors/500.html";
        case 501: return "./errors/501.html";
        case 503: return "./errors/503.html";
        case 504: return "./errors/504.html";
        case 505: return "./errors/505.html";
        default: return "./errors/501.html";
    }
}

bool isDigits(const std::string &str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
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
 *uncomment to debug
 * @brief Convert an integer to a string
 * @param n
 * @return std::string


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
void printsingleCharAndSpecialChar(const char c) {
        if (std::isprint(c)) {
            std::cout << c << ' ';
        } else {
//            std::cout<<CYAN<<"print special char" << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << ' '<<RESET_COLOR<<std::endl;
            std::cout<< "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << ' ';
        }
//    std::cout << '\n';
}
 */
