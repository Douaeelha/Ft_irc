#include "Server.hpp"

// Constructor
Server::Server(int port, std::string password) : _port(port), _password(password), _server_fd(-1)
{
    initSocket();
}

// Destructor
Server::~Server()
{
    // Clean up the main socket if it was successfully opened
    if (_server_fd != -1)
        close(_server_fd);
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
    std::cout << "Server listening on port " << _port << std::endl;

    while (true)
    {
        // 1. The switchboard operator waits for a light to flash
        // The -1 timeout means "wait forever until something happens"
        int poll_count = poll(_poll_fds.data(), _poll_fds.size(), -1); //poll() : It is a system call that asks the kernel: "I have an array of sockets. Put my program to sleep. Wake me up the exact millisecond any of them are ready to be read or written to."

        if (poll_count < 0)
            throw std::runtime_error("Error: poll() failed");

        // 2. A light flashed! Loop through the monitors to find which one
        for (size_t i = 0; i < _poll_fds.size(); i++)
        {
            // If revents is 0, this specific socket did nothing. Skip it.
            if (_poll_fds[i].revents == 0)
                continue;

            // Check if the event is data ready to read (POLLIN)
            if (_poll_fds[i].revents & POLLIN) // POLLIN : It means there is data waiting to be read
            {
                if (_poll_fds[i].fd == _server_fd) // It's the main door! Someone is trying to connect
                {
                    acceptNewClient();
                }
    
                else // It's an existing client! They sent a message
                {
                    receiveData(_poll_fds[i].fd);
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
    client_poll_fd.events = POLLIN; // Watch this client for incoming messages
    client_poll_fd.revents = 0;

    _poll_fds.push_back(client_poll_fd);

    std::cout << "New client connected! FD: " << client_fd << std::endl;
}

void Server::receiveData(int client_fd)
{
    char buffer[1024];
    
    // Call recv() to pull data from the socket into the bucket
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    // Scenario 1: We successfully read data
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the string so C++ knows where it ends
        std::cout << "Client " << client_fd << " sent: " << buffer;

        //Send a reply ---
        std::string reply = "Server says: I got your message!\r\n";
    
        send(client_fd, reply.c_str(), reply.length(), 0); // send() takes the fd, the raw string (c_str), the length, and 0 for flags
    }

    // Scenario 2: The client disconnected
    else if (bytes_received == 0)
    {
        std::cout << "Client " << client_fd << " disconnected." << std::endl;
        
        // Clean up this client
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

    // Scenario 3: An error occurred
    else
    {
        std::cerr << "Error: recv() failed on client " << client_fd << std::endl;
        
        // Clean up this client
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