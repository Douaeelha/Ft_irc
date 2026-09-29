#pragma once
#include <string>
#include <vector>
#include "Client_bonus.hpp"
#include <map>
#include <set>


class Channel{
    private:
        std::string _name;
        std::string _topic;
        std::string _key; // channel password (+k)
        // mode + x
        bool _inviteOnly; // mode +i , if true only members on invited set can join channel
        bool _topicRestricted; // mode +t , if true only operators can change topic
        bool _hasKey;// mmode +k set/unset
        bool _hasUserLimit;// mode + l set/unset
        size_t _userLimit; // only set if _hasUserLimit is true 
        // members 
        std::map<int, Client*> _members; // map of client file descriptors to Client pointers
        std::set<int> _operators; // members who hold operator status (@prefix, mode +o)
        std::set<int> _invited; // if _inviteOnly is true only clients in this set can join 
        public: 
            Channel(const std::string &name);
            ~Channel();
            Channel(const Channel &other);
            Channel &operator=(const Channel &other);
            // methods 
            //identity 
            std::string getName() const;

            //topic
            std::string getTopic() const;
            void setTopic(const std::string &topic);

            //mebers management
            void addMember(Client *client);
            void removeMember(int client_fd);
            bool isMember(int client_fd) const;
            size_t getMemberCount() const;
            const std::map<int, Client*> &getMembers() const;
        // operators management
            void addOperator(int fd);
            void removeOperator(int fd);
            bool isOperator(int fd) const;
        // invites 
            void inviteClient(int fd);
            void removeInvite(int fd);
            bool isInvited(int fd) const;
        // modes : i
        bool isInviteOnly() const;
        void setInviteOnly(bool value);
        // modes : t
        bool isTopicRestricted() const;
        void setTopicRestricted(bool value);
        //modes : k
        bool hasKey() const;
        void setKey(const std::string &key);
        std::string getKey() const;
        void removeKey();
        //modes : l
        bool hasUserLimit() const;
        void setUserLimit(size_t limit);
        size_t getUserLimit() const;
        void removeUserLimit();
        // messaging:
        void broadcast(const std::string &message, int exclude_fd = -1) const;

    };