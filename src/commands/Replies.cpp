#include "Replies.hpp"

/*
params :
    serverName : name of the server
    code : reply code (3 digit string)
    params : parameters for the reply (space separated)
    message : message to be sent to the client
Builds an IRC reply message in the format:
    :<serverName> <code> <params> :<message>\r\n
*/
std::string Replies::build(const std::string &serverName, const std::string &code, const std::string &params, const std::string &message){
    return ":" + serverName + " " + code + " " + params + " :" + message + "\r\n";
}