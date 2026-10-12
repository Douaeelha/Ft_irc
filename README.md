*This project has been created as part of the 42 curriculum by achoukri, delhajou, rben-ais.*

# FT_IRC

## Description
This project is an Internet Relay Chat (IRC) server written in C++98. The goal of the project is to implement a functional, non-blocking IRC server capable of handling multiple client connections simultaneously using a single `poll()` (or equivalent) multiplexing loop without hanging or forking. 

The server allows clients to connect via TCP/IP, authenticate with a password, register a nickname/username, join channels, exchange public and private messages, and execute channel management operations with operator privileges.

---

## Instructions

### Compilation
Compile the project using the provided `Makefile`:
```bash
make
```

Compiled with `c++` using `-Wall -Wextra -Werror -std=c++98`.

### Execution

Run the server by providing a listening port and a connection password:

```bash
./ircserv <port> <password>

```

* **`<port>`**: The port number on which the IRC server will listen for incoming connections (e.g., `6667`).


* **`<password>`**: The connection password required for clients to authenticate with the server.



### Testing & Usage

You can connect using an official reference IRC client (such as HexChat, Irssi, or KvIRC) or via `nc` (netcat):

```bash
nc -C 127.0.0.1 <port>
PASS <password>
NICK <nickname>
USER <username> 0 * :<realname>
JOIN #<channel>

```

---

## Mandatory Features

* **Authentication & User Registration**: `PASS`, `NICK`, `USER`, `QUIT`

* **Messaging & Channels**: Private messaging (`PRIVMSG`), `JOIN`, public channel broadcasting


* **Operator Commands**:
* `KICK`: Eject a client from a channel


* `INVITE`: Invite a client to a channel


* `TOPIC`: View or change the channel topic


* `MODE`: Manage channel modes:
* `i`: Set/remove Invite-only channel


* `t`: Set/remove Topic restrictions for channel members 


* `k`: Set/remove Channel key (password)


* `o`: Give/take Channel operator privilege


* `l`: Set/remove User limit to channel

---

## Resources

### References

* [RFC 1459 — Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459)
* [Modern IRC Client Protocol Documentation](https://modern.ircdocs.horse/)
* [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf)
* [The chirc Project Documentation](http://chi.cs.uchicago.edu/chirc/index.html)

### AI Usage

As required by the curriculum, AI tools were utilized for the following tasks:

* Assisting with understanding socket multiplexing concepts and standard TCP state transitions.

* Clarifying edge-case interpretations in RFC 1459 numeric reply standards.
* Assisting in structuring initial documentation and formatting templates.
* Reviewing and refining debugging workflows for non-blocking I/O handling.

