cmake_minimum_required(VERSION 3.27)
project(webserver)
set(CMAKE_CXX_STANDARD 98)
#set(CMAKE_CXX_STANDARD 14)

include_directories(./include ./src)

add_executable(webserver
        Client.cpp
        Client.hpp
        include.h
        StateMachine.hpp
        main.cpp
        ConfigParser.cpp
        ConfigParser.hpp
        ConfigBlock.cpp
        ConfigBlock.hpp
        Request.cpp
        Request.hpp
        Response.cpp
        Response.hpp
        Server.cpp
        Server.hpp
        Socket.cpp
        Socket.hpp
        Location.cpp
        Location.hpp
        Webserver.hpp
        Webserver.cpp
)
