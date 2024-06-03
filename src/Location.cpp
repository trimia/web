#include "../include/Location.hpp"

Location::Location()
{
    this->_autoindex = false;
//	std::cout << "Location : Default Constructor Called" << std::endl;
}


//Location::Location(const std::string &path, const std::string &root, bool autoindex, const std::string &index,
//                   std::allocator<std::basic_string<char> > methods, std::allocator<std::basic_string<char> > ret, std::string alias, std::vector<std::string> cgiPath,
//                   std::vector<std::string> cgiExt, unsigned long  clientMaxBodySize):
//                   _path(path), _root(root), _autoindex(autoindex), _index(index),
//                   _methods(methods), _return(ret), _alias(alias), _cgiPath(cgiPath),
//                   _cgiExt(cgiExt), _clientMaxBodySize(reinterpret_cast<const char *>(clientMaxBodySize)) {}


Location::~Location()
{
//	std::cout << "Location : Destructor Called" << std::endl;
}

Location::Location(Location const &obj)
{
//	std::cout << "Copy Constructor Called" << std::endl;
	if (this != &obj)
		*this = obj;
}

Location	&Location::operator= (const Location &obj)
{
//	std::cout << "Copy Assignment Operator Called" << std::endl;
    if (this != &obj)
    {
        this->_path=obj._path;
        this->_root=obj._root;
        this->_autoindex=obj._autoindex;
        this->_index=obj._index;
        this->setMethods(obj._methods);
        this->_cgiPath=obj._cgiPath;
        this->_alias=obj._alias;
        this->_clientMaxBodySize=obj._clientMaxBodySize;
        //        this->_error_pages=obj._error_pages;
    }
    return (*this);
}



// Location Location::fitBestLocation(Client *client) {
//     Location bestMatch;
//     size_t bestMatchLenght = 0;
//     // Itera attraverso le posizioni definite nel server
//     for (std::vector<Location>::iterator it = client->locations().begin(); it != client->locations().end(); it++) {
//         if (client->request()->path_file().find(it->getPath()) == 0 && it->getPath().length() > bestMatchLenght) {
//             bestMatch = *it.base();
//             bestMatchLenght = it->getPath().length();
//         }
//     }
//     return bestMatch;
// }

bool Location::allowMethod(std::string method) {
    std::vector<std::string> methods = this->getMethods();
    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (it->compare(method) == 0)
            return true;
    }
    return false;
}
//
bool Location::autoIndex(std::string path){
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        std::cout << RED << "stat() error" << RESET_COLOR << std::endl;
        return false;
    }
    if(S_ISDIR(info.st_mode)){
        return true;
    }
    return false;
}
//
std::string Location::generateDirectoryListing(const std::string& path) {
    DIR* dir;
    struct dirent* ent;
    std::ostringstream html;

    html << "<html><body><ul>";

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            html << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>";
        }
        closedir(dir);
    } else {
        // Could not open directory
        return "";
    }

    html << "</ul></body></html>";

    return html.str();
}


/*
 *			getter & setter
 */

std::string & Location::root() {
    return _root;
}

bool & Location::getAutoIndex() {
    return _autoindex;
}

std::string & Location::index() {
    return _index;
}

std::vector<std::string> & Location::cgiPath() {
    return _cgiPath;
}

std::string & Location::alias() {
    return _alias;
}

void Location::set_alias(std::string alias) {
    _alias = alias;
}

std::basic_string<char> & Location::clientMaxBodySize() {
    return _clientMaxBodySize;
}

std::vector<std::string> & Location::getReturn() {
    return _return;
}

void Location::set_return(std::vector<std::string> return_) {
    _return = return_;
}

std::vector<std::string> & Location::cgi_ext() {
    return _cgiExt;
}

void Location::set_cgi_ext(std::vector<std::string> cgi_ext) {
    _cgiExt = cgi_ext;
}

void Location::setPath(std::string path) {
    this->_path = path;
}

std::string Location::getPath() {
    return this->_path;
}

void Location::setRoot(std::string &root) {
    this->_root = root;
}

void Location::setIndex(std::string &index) {
    this->_index = index;
}

void Location::setAutoIndex(bool autoindex) {
    this->_autoindex = autoindex;
}

void Location::setMethods(std::vector<std::string> methods) {
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++) {
        this->_methods.push_back(*it);
    }
}

std::vector<std::string> Location::getMethods() {
    return this->_methods;
}

void Location::setReturn(std::vector<std::string> &returnPage) {
    this->_return = returnPage;
}

void Location::setCgiPath(std::vector<std::string> &cgiPath) {
    this->_cgiPath = cgiPath;
}

void Location::setClientMaxBodySize(std::basic_string<char> clientMaxBodySize) {
    this->_clientMaxBodySize = clientMaxBodySize;
}
