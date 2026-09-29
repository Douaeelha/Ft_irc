#pragma once 
#include <string> 

// represent a single use

class Client{
    private:
        int _fd;// needed to know which socket to send response to
        std::string _nickname;// to be set by the nick command
        std::string _username; // to be set by the user command
        std::string _realname; // to be set by the user command
        //security flags 
        std::string _hostname; // to be set by the user command
        bool _hadPassword; // to be set by the pass command to check if user is to be accepted 
        bool _hasNickname; // to be set by the nick command to check if user is to be accepted
        bool _hasUsername; // to be set by the user command to check if user is
        //registration flags
        bool _is_registered;// have they completed the registration process
        // PASS NICK USER 
        std::string _buffer_in; // to store incoming messages from the client
        std::string _buffer_out; // to store outgoing messages to the client
    public:
        Client(int fd);
        ~Client();
        Client(const Client &other);
        Client &operator=(const Client &other);
        // getters 
        int getFd() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getRealname() const;
         bool getHadPassword() const;
        bool getHasNickname() const;
        bool getHasUsername() const;
        bool getIsRegistered() const;
        std::string getHostname() const;
        // setters
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setRealname(const std::string &realname);
        void setHadPassword(bool hadPassword);
        void setHasNickname(bool hasNickname);
        void setHasUsername(bool hasUsername);
        void setIsRegistered(bool isRegistered);
        void setHostname(const std::string &hostname);
      // buffer management
        void appendToBufferIn(const std::string &message);
        void appendToBufferOut(const std::string &message);
        std::string getBufferIn() const;
        std::string getBufferOut() const;
       
        void clearBufferIn();
        void clearBufferOut();
};
