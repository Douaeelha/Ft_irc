#include "Server.hpp"
#include "Dispatcher.hpp"

// Constructor
Server::Server(int port, std::string password)
    : _port(port), _password(password), _server_name("irc.local"), _server_fd(-1)
{
    initSocket();
}

// Destructor
Server::~Server()
{
    // Loop through all monitored sockets and close them
    for (size_t i = 0; i < _poll_fds.size(); i++)
    {
        if (_poll_fds[i].fd != -1)
        {
            close(_poll_fds[i].fd);
        }
    }
    
    std::cout << "All sockets closed. Memory cleaned up." << std::endl;
}

// Initialize the static variable
bool Server::_isRunning = true;

// The function the OS will call when you press Ctrl+C
void Server::signalHandler(int signum)
{
    (void)signum; // Cast to void to silence unused variable warnings
    std::cout << "\nSignal received. Shutting down server gracefully..." << std::endl;
    Server::_isRunning = false;
}

void    Server::initSocket()
{
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_server_fd < 0)
        throw std::runtime_error("Error: Failed to create socket");

    //multiplexing: one waiter does all , doesnt not wait on a table if there not ready 
    fcntl(_server_fd, F_SETFL, O_NONBLOCK);

    // Configure the address struct
    struct sockaddr_in  server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port); // "Host TO Network Short." You are flipping your port integer into Big-Endian format so the network stack reads it correctly.
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY (which translates to 0.0.0.0) tells your server to accept connections from any of your computer's IP addresses (localhost, Wi-Fi, Ethernet). htonl ("Host TO Network Long") flips it into Big-Endian.

    // Bind the socket to the IP and Port
    if (bind(_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Error: Failed to bind socket");

    // Start listening (SOMAXCONN lets the OS define the max queue size)
    if (listen(_server_fd, SOMAXCONN) < 0) // SOMAXCONN : (Socket Maximum Connections) is a built-in constant that tells the OS: "Make this waiting room the maximum safe size." (On most systems, this defaults to 128). If 129 people connect instantly, the 129th person immediately gets a "Connection Refused" error
        throw std::runtime_error("Error: Failed to listen on socket");


    //  Add the server socket to the poll vector to be watched
    struct pollfd   server_poll_fd;
    server_poll_fd.fd = _server_fd;
    server_poll_fd.events = POLLIN; // Tell OS: Notify me when there is data to read (a new connection)
    server_poll_fd.revents = 0;     // The OS will fill this in later, initialize to 0

    _poll_fds.push_back(server_poll_fd);
}

void Server::run()
{
    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);

    std::cout << "Server listening on port " << _port << std::endl;

    while (Server::_isRunning)
    {
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1); 

        if (poll_count < 0)
        {
            // If Ctrl+C was pressed, the flag is now false. Break the loop cleanly!
            if (!Server::_isRunning)
                break;
                
            throw std::runtime_error("Error: poll() failed");
        }
        
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
            if (_poll_fds[i].revents == 0)
                continue;

            // flag to track if the client was deleted during POLLIN
            bool client_disconnected = false;

            if (_poll_fds[i].revents & POLLIN) 
            {
                if (_poll_fds[i].fd == _server_fd) 
                {
                    acceptNewClient();
                }
                else 
                {
                    int current_fd = _poll_fds[i].fd;
                    receiveData(current_fd);
                    
                    // If the client is no longer in Douae's map, it was deleted!
                    if (_clients.find(current_fd) == _clients.end())
                    {
                        client_disconnected = true;
                    }
                }
            }
            
            // Safety escape hatch
            if (client_disconnected)
            {
                i--; // Step back so we don't skip the next socket in the vector
                continue; // Skip the POLLOUT check below
            }

            // Check if the socket is ready to be written to (POLLOUT)
            if (_poll_fds[i].fd != _server_fd && (_poll_fds[i].revents & POLLOUT))
            {
                int current_fd = _poll_fds[i].fd;
                
                std::map<int, Client>::iterator it = _clients.find(current_fd);
                if (it != _clients.end())
                {
                    std::string out_buffer = it->second.getBufferOut();
                    
                    if (!out_buffer.empty())
                    {
                        int bytes_sent = send(current_fd, out_buffer.c_str(), out_buffer.length(), 0);
                        
                        if (bytes_sent > 0)
                        {
                            it->second.clearBufferOut(); 
                        }
                        else
                        {
                            std::cerr << "Error: Failed to send data to client " << current_fd << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void Server::acceptNewClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        std::cerr << "Error: Failed to accept new client" << std::endl;
        return;
    }

    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // Pack it into a pollfd struct and add it to the watch list
    struct pollfd client_poll_fd;
    client_poll_fd.fd = client_fd;
    client_poll_fd.events = POLLIN | POLLOUT; // Watch this client for incoming messages
    client_poll_fd.revents = 0;

    _poll_fds.push_back(client_poll_fd);

    // Instantiate Douae's Client and store it in the map:
    _clients.insert(std::make_pair(client_fd, Client(client_fd)));

   // new: Grab the IP and give it to Douae's Client
    std::string client_ip = inet_ntoa(client_addr.sin_addr);
    std::map<int, Client>::iterator it = _clients.find(client_fd);
    if (it != _clients.end())
    {
        it->second.setHostname(client_ip);
    }    
    std::cout << "New client connected! FD: " << client_fd << " IP: " << client_ip << std::endl;
}

void Server::receiveData(int client_fd)
{
    char buffer[1024];
    
    // Call recv() to pull data from the socket into the bucket
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    // Scenario 1: We successfully read data
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        
        std::map<int, Client>::iterator it = _clients.find(client_fd);
        if (it != _clients.end())
        {
            // Push data to the client's buffer
            it->second.appendToBufferIn(buffer); 

            // Parse and dispatch complete IRC lines (ending with '\n').
            std::string input = it->second.getBufferIn();
            size_t line_end = input.find('\n');

            while (line_end != std::string::npos)
            {
                std::string raw_line = input.substr(0, line_end + 1);
                input.erase(0, line_end + 1);

                Parser::Command cmd = _parser.parse(raw_line);
                if (!cmd.name.empty())
                {
                    std::map<int, Client*> client_ptrs;
                    std::map<int, Client>::iterator cit = _clients.begin();
                    for (; cit != _clients.end(); ++cit)
                        client_ptrs[cit->first] = &cit->second;

                    Dispatcher::dispatch(cmd, it->second, client_ptrs, _channels, _password, _server_name);
                }

                line_end = input.find('\n');
            }

            // Keep an incomplete tail for the next recv() call.
            it->second.clearBufferIn();
            it->second.appendToBufferIn(input);
        }
    }
    // Scenario 2 & 3: The client disconnected OR an error occurred
    else
    {
        // Print the correct message
        if (bytes_received == 0)
            std::cout << "Client " << client_fd << " disconnected." << std::endl;
        else
            std::cerr << "Error: recv() failed on client " << client_fd << std::endl;
        
        // Clean up Douae's map 
        _clients.erase(client_fd);
        
        // Clean up the server socket and vector
        close(client_fd);
        for (std::vector<struct pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
        {
            if (it->fd == client_fd)
            {
                _poll_fds.erase(it);
                break;
            }
        }
    }
}