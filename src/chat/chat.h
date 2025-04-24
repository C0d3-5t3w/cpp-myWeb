#ifndef CHAT_H
#define CHAT_H

#include "message.h"
#include "storage.h"
#include <vector>
#include <string>
#include <mutex>

class ChatManager
{
private:
    std::vector<Message> messages_;
    Storage storage_;
    std::mutex mutex_;

public:
    ChatManager(const std::string &storage_path);
    void add_message(const std::string &text);
    std::vector<Message> get_messages() const;
    std::string get_messages_as_json_array() const;
};

#endif
