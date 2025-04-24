#ifndef WEB_H
#define WEB_H

#include <string>
#include "../chat/chat.h" // Include ChatManager

class WebServer
{
public:
    WebServer(const std::string &web_root, const std::string &ip_address, int port);

    ~WebServer();

    void run();

private:
    std::string web_root_;
    std::string ip_address_;
    int port_;
    int server_fd_;
    ChatManager chat_manager_; // Add ChatManager instance

    void handle_client(int client_socket);

    void send_response(int client_socket, const std::string &status, const std::string &content_type, const std::string &content);

    std::string read_file(const std::string &path);

    std::string get_mime_type(const std::string &path);
};

#endif
