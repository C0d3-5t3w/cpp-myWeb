#include "web/web.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

const std::string DEFAULT_WEB_ROOT = "../src/web";

int main()
{
    std::string ip_address;
    int port;

    std::cout << "Enter the IP address to bind to (e.g., 127.0.0.1): ";
    std::cin >> ip_address;

    std::cout << "Enter the port number to listen on (e.g., 8080): ";
    while (!(std::cin >> port) || port <= 0 || port > 65535)
    {
        std::cout << "Invalid port. Please enter a number between 1 and 65535: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    try
    {

        WebServer server(DEFAULT_WEB_ROOT, ip_address, port);
        server.run();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Server failed to start: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
