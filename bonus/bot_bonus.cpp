#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

void sendData(int fd, const std::string& msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
    std::cout << "[Bot Sent] " << msg;
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cerr << "Usage: ./ircbot <ip> <port> <password> <bot_name>\n";
        return 1;
    }

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);
    std::string password = argv[3];
    std::string botName = argv[4];
    

    //connect
    int bot_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bot_fd < 0)
    {
        std::cerr << "Error: Could not create socket\n";
        return 1;
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    if (connect(bot_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Error: Connection failed\n";
        return 1;
    }

    //Register
    sendData(bot_fd, "PASS " + password + "\r\n");
    sendData(bot_fd, "NICK " + botName + "\r\n");
    sendData(bot_fd, "USER " + botName + " 0 * :IRC Helper Bot\r\n");
    
    // Join a default channel
    sendData(bot_fd, "JOIN #general\r\n");

    //The Listen & React Loop
    char buffer[1024];
    while (1337)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(bot_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes <= 0)
        {
            std::cout << "Server disconnected.\n";
            break;
        }

        std::string msg(buffer);
        std::cout << "Bot received: " << msg;
    
        if (msg.find("PRIVMSG") != std::string::npos)
        {
            // Extract the Sender (The nick between the starting ':' and the '!')
            size_t excl_pos = msg.find("!");
            std::string sender = "";
            if (msg[0] == ':' && excl_pos != std::string::npos)
            {
                sender = msg.substr(1, excl_pos - 1);
            }

            //Extract the Target (The channel or nick right after "PRIVMSG ")
            size_t target_start = msg.find("PRIVMSG ") + 8;
            size_t target_end = msg.find(" ", target_start);
            std::string target = "";
            if (target_start != std::string::npos && target_end != std::string::npos) {
                target = msg.substr(target_start, target_end - target_start);
            }

            //Extract the Message Text (Everything after " :")
            size_t text_start = msg.find(" :", target_end);
            std::string text = "";
            if (text_start != std::string::npos) {
                text = msg.substr(text_start + 2); // +2 skips the " :"
            }

            //Dynamic Routing: Reply to the sender if it was a DM, otherwise reply to the channel
            std::string reply_to = (target == botName) ? sender : target;

            //Commands
            if (text.find("!joke") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + " :\r\n");
            }

            else if (text.find("!secret_joke") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + " سعيد جماني دار أوطو سطوب وقف ليه كاميون هاز لبقر. المهم في الطريق طاحت واحد لبقرة. مني وصلو حسبو البقرات لقاو بقرة ناقصة. قالو لسعيد جماني مالك ماقلت لينا البقرة طاحت. قاليهم كون طحت أنا غادي تقولها ليكم البقرة.:\r\n");
            }

            else if (text.find("!who") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + "u know who:\r\n");
            }

            else if (text.find("who") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + "01101100 01101001 00100000 00110111 01110111 01100001 01101011:\r\n");
            }

            else if (text.find("!link") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + "check out this Oxford study: https://youtu.be/dQw4w9WgXcQ:\r\n");
            }

            else if (text.find("!ping") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + " :Pong! I am alive and listening.\r\n");
            }

            else if (text.find("!time") == 0)
            {
                time_t now = time(0);
                std::string dt = ctime(&now);
                dt.erase(dt.length() - 1); // Clean up the newline that ctime adds
                sendData(bot_fd, "PRIVMSG " + reply_to + " :Server time is " + dt + "\r\n");
            }
            else if (text.find("!help") == 0)
            {
                sendData(bot_fd, "PRIVMSG " + reply_to + " :Available commands: !joke, !ping, !time, !who, !link, !help\r\n");
            }
        }

    }
    
    close(bot_fd);
    return 0;
}