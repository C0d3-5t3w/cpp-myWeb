#include "web.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void log_error(const std::string &msg)
{
    std::cerr << "Error: " << msg << " - " << strerror(errno) << std::endl;
}

WebServer::WebServer(const std::string &web_root, const std::string &ip_address, int port)
    : web_root_(web_root), ip_address_(ip_address), port_(port), server_fd_(-1)
{

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {

        log_error("setsockopt(SO_REUSEADDR) failed");
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);

    if (inet_pton(AF_INET, ip_address_.c_str(), &server_addr.sin_addr) <= 0)
    {
        close(server_fd_);
        throw std::runtime_error("Invalid IP address format");
    }

    if (bind(server_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(server_fd_);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(server_fd_, 10) < 0)
    {
        close(server_fd_);
        throw std::runtime_error("Failed to listen on socket");
    }

    std::cout << "Server listening on " << ip_address_ << ":" << port_ << "..." << std::endl;
    std::cout << "Serving files from: " << web_root_ << std::endl;
}

WebServer::~WebServer()
{
    if (server_fd_ >= 0)
    {
        close(server_fd_);
        std::cout << "Server socket closed." << std::endl;
    }
}

void WebServer::run()
{
    while (true)
    {

        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd_, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0)
        {
            log_error("Failed to accept connection");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "Connection accepted from " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        handle_client(client_socket);

        close(client_socket);
        std::cout << "Connection closed for " << client_ip << std::endl;
    }
}

void WebServer::handle_client(int client_socket)
{
    const int buffer_size = 4096;
    std::vector<char> buffer(buffer_size);
    std::string request_str;

    int bytes_read = read(client_socket, buffer.data(), buffer_size - 1);
    if (bytes_read < 0)
    {
        log_error("Failed to read from client socket");
        return;
    }
    if (bytes_read == 0)
    {
        std::cout << "Client disconnected before sending data." << std::endl;
        return;
    }

    buffer[bytes_read] = '\0';
    request_str = buffer.data();

    size_t first_line_end = request_str.find("\r\n");
    if (first_line_end == std::string::npos)
    {
        send_response(client_socket, "400 Bad Request", "text/plain", "Bad Request");
        return;
    }
    std::string request_line = request_str.substr(0, first_line_end);
    std::cout << "Request: " << request_line << std::endl;

    std::stringstream ss(request_line);
    std::string method, path, http_version;
    ss >> method >> path >> http_version;

    if (method != "GET")
    {
        send_response(client_socket, "501 Not Implemented", "text/plain", "Only GET method is supported");
        return;
    }

    if (path == "/")
    {
        path = "/index.html";
    }

    std::string file_path = web_root_ + path;

    std::string file_content = read_file(file_path);

    if (file_content.empty())
    {

        std::ifstream test_file(file_path);
        if (test_file.good())
        {
            send_response(client_socket, "500 Internal Server Error", "text/plain", "Could not read file");
        }
        else
        {
            send_response(client_socket, "404 Not Found", "text/plain", "File Not Found");
        }
    }
    else
    {

        std::string mime_type = get_mime_type(file_path);
        send_response(client_socket, "200 OK", mime_type, file_content);
    }
}

std::string WebServer::read_file(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        log_error("Failed to open file: " + path);
        return "";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(size, '\0');
    if (!file.read(&content[0], size))
    {
        log_error("Failed to read file: " + path);
        return "";
    }

    return content;
}

std::string WebServer::get_mime_type(const std::string &path)
{
    size_t dot_pos = path.rfind('.');
    if (dot_pos == std::string::npos)
    {
        return "application/octet-stream";
    }

    std::string ext = path.substr(dot_pos);
    if (ext == ".html" || ext == ".htm")
        return "text/html";
    if (ext == ".css")
        return "text/css";
    if (ext == ".js")
        return "application/javascript";
    if (ext == ".jpg" || ext == ".jpeg")
        return "image/jpeg";
    if (ext == ".png")
        return "image/png";
    if (ext == ".gif")
        return "image/gif";
    if (ext == ".txt")
        return "text/plain";

    return "application/octet-stream";
}

void WebServer::send_response(int client_socket, const std::string &status, const std::string &content_type, const std::string &content)
{
    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 " << status << "\r\n";
    response_stream << "Content-Type: " << content_type << "\r\n";
    response_stream << "Content-Length: " << content.length() << "\r\n";
    response_stream << "Connection: close\r\n";
    response_stream << "\r\n";
    response_stream << content;

    std::string response_str = response_stream.str();

    ssize_t bytes_sent = write(client_socket, response_str.c_str(), response_str.length());
    if (bytes_sent < 0)
    {
        log_error("Failed to write response to socket");
    }
    else if (static_cast<size_t>(bytes_sent) < response_str.length())
    {
        std::cerr << "Warning: Could not send entire response." << std::endl;
    }
}
