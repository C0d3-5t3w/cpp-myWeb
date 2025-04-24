#include "chat.h"
#include <sstream>

ChatManager::ChatManager(const std::string &storage_path)
    : storage_(storage_path)
{
    messages_ = storage_.load_messages();
}

void ChatManager::add_message(const std::string &text)
{
    std::lock_guard<std::mutex> lock(mutex_);
    messages_.emplace_back(text);
    storage_.save_messages(messages_);
}

std::vector<Message> ChatManager::get_messages() const
{

    return messages_;
}

std::string ChatManager::get_messages_as_json_array() const
{

    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < messages_.size(); ++i)
    {
        ss << messages_[i].serialize();
        if (i < messages_.size() - 1)
        {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
