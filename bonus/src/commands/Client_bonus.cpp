#include "Client_bonus.hpp"

Client::Client(int fd) {
    this->_fd = fd;
    this->_nickname = "";
    this->_username = "";
    this->_realname = "";
    
    this->_hadPassword = false;
    this->_hasNickname = false;
    this->_hasUsername = false;
    this->_is_registered = false;
    
    this->_buffer_in = "";
    this->_buffer_out = "";
}

Client::~Client() {}

Client::Client(const Client &other) {
    *this = other;
}

Client &Client::operator=(const Client &other) {
    if (this != &other) {
        this->_fd = other._fd;
        this->_nickname = other._nickname;
        this->_username = other._username;
        this->_realname = other._realname;
        
        this->_hadPassword = other._hadPassword;
        this->_hasNickname = other._hasNickname;
        this->_hasUsername = other._hasUsername;
        this->_is_registered = other._is_registered;
        
        this->_buffer_in = other._buffer_in;
        this->_buffer_out = other._buffer_out;
    }
    return *this;
}

int Client::getFd() const {
    return this->_fd;
}

std::string Client::getNickname() const {
    return this->_nickname;
}

std::string Client::getUsername() const {
    return this->_username;
}

std::string Client::getRealname() const {
    return this->_realname;
}

bool Client::getHadPassword() const {
    return this->_hadPassword;
}

bool Client::getHasNickname() const {
    return this->_hasNickname;
}

bool Client::getHasUsername() const {
    return this->_hasUsername;
}

bool Client::getIsRegistered() const {
    return this->_is_registered;
}

std::string Client::getHostname() const {
    return this->_hostname;
}

void Client::setNickname(const std::string &nickname) {
    this->_nickname = nickname;
}

void Client::setUsername(const std::string &username) {
    this->_username = username;
}

void Client::setRealname(const std::string &realname) {
    this->_realname = realname;
}

void Client::setHadPassword(bool hadPassword) {
    this->_hadPassword = hadPassword;
}

void Client::setHasNickname(bool hasNickname) {
    this->_hasNickname = hasNickname;
}

void Client::setHasUsername(bool hasUsername) {
    this->_hasUsername = hasUsername;
}

void Client::setHostname(const std::string &hostname) {
    this->_hostname = hostname;
}

void Client::setIsRegistered(bool isRegistered) {
    this->_is_registered = isRegistered;
}

void Client::appendToBufferIn(const std::string &message) {
    this->_buffer_in += message;
}

std::string Client::getBufferIn() const {
    return this->_buffer_in;
}

void Client::clearBufferIn() {
    this->_buffer_in.clear();
}

void Client::appendToBufferOut(const std::string &message) {
    this->_buffer_out += message;
}

std::string Client::getBufferOut() const {
    return this->_buffer_out;
}

void Client::clearBufferOut() {
    this->_buffer_out.clear();
}
