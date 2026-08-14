#include "Dispatcher.hpp"
#include "Commands.hpp"
#include "Replies.hpp"
#include <cctype>
#include "Parser.hpp"

static std::string toUpper(const std::string &s) {
    std::string result = s;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[i])));
    return result;
}

void Dispatcher::dispatch(const Parser::Command &cmd, Client &client,
                           std::map<int, Client*> &clients,
                           std::map<std::string, Channel> &channels,
                           const std::string &serverPassword,
                           const std::string &serverName) {
    // IRC command names are case-insensitive (RFC 2812 2.3.1) -- "nick",
    // "Nick", "NICK" must all work identically.
    std::string name = toUpper(cmd.name);

    // These are the only commands a not-yet-registered client is allowed to
    // send. Everything else gets ERR_NOTREGISTERED until PASS+NICK+USER all
    // succeed.
    bool isPreRegistrationCommand =
        (name == "PASS" || name == "NICK" || name == "USER" ||
         name == "QUIT" || name == "PING");

    if (!client.getIsRegistered() && !isPreRegistrationCommand) {
        std::string target = client.getNickname().empty() ? "*" : client.getNickname();
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_NOTREGISTERED,
                                                  target, "You have not registered"));
        return;
    }

    if (name == "PASS") {
        Commands::handlePass(client, cmd.params, serverPassword, serverName);
    } else if (name == "NICK") {
        Commands::handleNick(client, cmd.params, clients, serverName);
    } else if (name == "USER") {
        Commands::handleUser(client, cmd.params, serverName);
    } else if (name == "JOIN") {
        Commands::handleJoin(client, cmd.params, channels, serverName);
    } else if (name == "PRIVMSG") {
        Commands::handlePrivmsg(client, cmd.params, clients, channels, serverName);
    } else if (name == "KICK") {
        Commands::handleKick(client, cmd.params, channels, serverName);
    } else if (name == "INVITE") {
        Commands::handleInvite(client, cmd.params, clients, channels, serverName);
    } else if (name == "TOPIC") {
        Commands::handleTopic(client, cmd.params, channels, serverName);
    } else if (name == "MODE") {
        Commands::handleMode(client, cmd.params, clients, channels, serverName);
    } else if (name == "QUIT") {
        Commands::handleQuit(client, cmd.params, channels, serverName);
    } else if (name == "PING") {
        // keep-alive: not in Commands.hpp since it needs no channel/client
        // state at all, just echoes the token back as PONG
        std::string token = cmd.params.empty() ? serverName : cmd.params[0];
        client.appendToBufferOut(":" + serverName + " PONG " + serverName + " :" + token + "\r\n");
    } else {
        std::string target = client.getNickname().empty() ? "*" : client.getNickname();
        client.appendToBufferOut(Replies::build(serverName, Replies::ERR_UNKNOWNCOMMAND,
                                                  target + " " + cmd.name, "Unknown command"));
    }

    // registration only ever completes as a *result* of PASS/NICK/USER, so
    // only bother re-checking after those three
    if (name == "PASS" || name == "NICK" || name == "USER")
        Commands::checkRegistrationComplete(client, serverName);
}