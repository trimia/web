#ifndef _Location_HPP_
#define _Location_HPP_

#include "include.h"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //

class	Location
{
	public	:
		Location ();

		Location (Location const &obj);



        Location(const std::string &path, const std::string &root, bool autoindex, const std::string &index,
                 std::allocator<std::basic_string<char> > methods, std::allocator<std::basic_string<char> > ret, std::string alias, std::vector<std::string> cgiPath,
                 std::vector<std::string> cgiExt, unsigned long clientMaxBodySize);

        ~Location ();
        Location &operator= (const Location &obj);

        void                        setPath(std::string &path);
        void                        setRoot(std::string &root);
        void                        setIndex(std::string &index);
        void                        setAutoIndex(bool autoindex);
        void                        setMethods(std::vector<std::string> methods);
        void                        setReturn(std::vector<std::string> &returnPage);
        void                        setCgiPath(std::vector<std::string> &cgiPath);
        void                        setClientMaxBodySize(std::basic_string<char> clientMaxBodySize);

private	:
    //check if is all necessary and to do what?
        std::string					_path;
        std::string					_root;
        bool						_autoindex;
        std::string					_index;
        std::vector<std::string>    _methods; // GET+ POST- DELETE- PUT- HEAD-
        std::vector<std::string>	_return;
        std::string					_alias;
        //is necessary?
        std::vector<std::string>	_cgiPath;
        std::vector<std::string>	_cgiExt;
        std::basic_string<char>     _clientMaxBodySize;
    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
