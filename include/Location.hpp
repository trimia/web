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
             const std::vector<short> &methods, std::string ret, std::string alias, std::vector<std::string> cgiPath,
             std::vector<std::string> cgiExt, unsigned long clientMaxBodySize);

    ~Location ();
		Location &operator= (const Location &obj);

	private	:
    //check if is all necessary and to do what?
        std::string					_path;
        std::string					_root;
        bool						_autoindex;
        std::string					_index;
        std::vector<short>			_methods; // GET+ POST- DELETE- PUT- HEAD-
        std::string					_return;
        std::string					_alias;
        //is necessary?
        std::vector<std::string>	_cgiPath;
        std::vector<std::string>	_cgiExt;
        unsigned long				_clientMaxBodySize;
    //	DataType	attributes.
};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
