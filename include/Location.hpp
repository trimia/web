#ifndef _Location_HPP_
#define _Location_HPP_

#include "include.h"
//#include "Client.hpp"

// ******************************************************** //
//                         CLASSES                         //
// ****************************************************** //

class	Location
{
	public	:
		Location ();

		Location (Location const &obj);

        ~Location ();
        Location &operator= (const Location &obj);
//        Location fitBestLocation(std::vector<Location> locations, std::string path_file);
		bool allowMethod(std::string method);

        bool autoIndex(std::string path);

        std::string generateDirectoryListing(std::string path,std::string root);


		std::string & root();

		bool & getAutoIndex();

		std::string & index();

		std::vector<std::string> & cgiPath();

		std::string & alias();

		void set_alias(std::string alias);

		std::basic_string<char> & clientMaxBodySize();

    bool isIsRoot() const;

    void setIsRoot(bool isRoot);

    // void set_return(std::vector<std::string> return_);

		std::vector<std::string> & cgi_ext();

		void set_cgi_ext(std::vector<std::string> cgi_ext);

		void                        setPath(std::string path);
        std::string                 getPath(void);
        void                        setRoot(std::string &root);
        void                        setIndex(std::string &index);
        void                        setAutoIndex(bool autoindex);
        void                        setMethods(std::vector<std::string> methods);
        std::vector<std::string>    getMethods(void);
        void                        setReturn(std::vector<std::string> returnPage);
        std::vector<std::string>    getReturn(void);
//        void                        setCgiPath(std::vector<std::string> &cgiPath);
        void                        setClientMaxBodySize(std::basic_string<char> clientMaxBodySize);

    bool getIsCgi() const;

    void setIsCgi(bool isCgi);

private	:
    //check if is all necessary and to do what?
        // int                         _id;
        std::string					_path;
        std::string					_root;
        bool                        _isRoot;
        bool						_autoindex;
        std::string					_index;
        std::vector<std::string>    _methods; // GET+ POST- DELETE- PUT- HEAD-
        bool	                    _isCgi;
        std::string					_alias;
		std::basic_string<char>     _clientMaxBodySize;
		std::vector<std::string>	_return;
		//is necessary?
		std::vector<std::string>	_cgiExt;
        //	DataType	attributes.

};

// ******************************************************** //
//                        FUNCTIONS                        //
// ****************************************************** //

#endif
