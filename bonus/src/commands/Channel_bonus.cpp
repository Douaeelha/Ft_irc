#include "Channel_bonus.hpp"

Channel::Channel(const std::string &name) {
    this->_name = name;
    this->_topic = "";
    this->_key = "";

    this->_inviteOnly = false;
    this->_topicRestricted = true;
    this->_hasKey = false;
    this->_hasUserLimit = false;
    this->_userLimit = 0;
}

Channel::~Channel() {}

Channel::Channel(const Channel &other) {
    *this = other;
}

Channel &Channel::operator=(const Channel &other) {
    if (this != &other) {
        this->_name = other._name;
        this->_topic = other._topic;
        this->_key = other._key;

        this->_inviteOnly = other._inviteOnly;
        this->_topicRestricted = other._topicRestricted;
        this->_hasKey = other._hasKey;
        this->_hasUserLimit = other._hasUserLimit;
        this->_userLimit = other._userLimit;

        this->_members = other._members;
        this->_operators = other._operators;
        this->_invited = other._invited;
    }
    return *this;
}

std::string Channel::getName() const {
    return this->_name;
}

std::string Channel::getTopic() const {
    return this->_topic;
}

void Channel::setTopic(const std::string &topic) {
    this->_topic = topic;
}

void Channel::addMember(Client *client) {
    this->_members[client->getFd()] = client;
}

void Channel::removeMember(int client_fd) {
    this->_members.erase(client_fd);
    this->_operators.erase(client_fd);
    this->_invited.erase(client_fd);
}

bool Channel::isMember(int client_fd) const {
    return this->_members.find(client_fd) != this->_members.end();
}

size_t Channel::getMemberCount() const {
    return this->_members.size();
}

const std::map<int, Client*> &Channel::getMembers() const {
    return this->_members;
}

void Channel::addOperator(int fd) {
    this->_operators.insert(fd);
}

void Channel::removeOperator(int fd) {
    this->_operators.erase(fd);
}

bool Channel::isOperator(int fd) const {
    return this->_operators.find(fd) != this->_operators.end();
}

void Channel::inviteClient(int fd) {
    this->_invited.insert(fd);
}

void Channel::removeInvite(int fd) {
    this->_invited.erase(fd);
}

bool Channel::isInvited(int fd) const {
    return this->_invited.find(fd) != this->_invited.end();
}

bool Channel::isInviteOnly() const {
    return this->_inviteOnly;
}

void Channel::setInviteOnly(bool value) {
    this->_inviteOnly = value;
}

bool Channel::isTopicRestricted() const {
    return this->_topicRestricted;
}

void Channel::setTopicRestricted(bool value) {
    this->_topicRestricted = value;
}

bool Channel::hasKey() const {
    return this->_hasKey;
}

void Channel::setKey(const std::string &key) {
    this->_key = key;
    this->_hasKey = true;
}

std::string Channel::getKey() const {
    return this->_key;
}

void Channel::removeKey() {
    this->_key = "";
    this->_hasKey = false;
}

bool Channel::hasUserLimit() const {
    return this->_hasUserLimit;
}

void Channel::setUserLimit(size_t limit) {
    this->_userLimit = limit;
    this->_hasUserLimit = true;
}

size_t Channel::getUserLimit() const {
    return this->_userLimit;
}

void Channel::removeUserLimit() {
    this->_userLimit = 0;
    this->_hasUserLimit = false;
}

void Channel::broadcast(const std::string &message, int exclude_fd) const {
    std::map<int, Client*>::const_iterator it = this->_members.begin();
    for (; it != this->_members.end(); ++it) {
        if (it->first != exclude_fd)
            it->second->appendToBufferOut(message);
    }
}