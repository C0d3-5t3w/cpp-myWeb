#include "chat.h"
#include <sstream> // For get_messages_as_json_array

ChatManager::ChatManager(const std::string& storage_path)
    : storage_(storage_path) {
    messages_ = storage_.load_messages();
}

void ChatManager::add_message(const std::string& text) {
    std::lock_guard<std::mutex> lock(mutex_); // Lock for thread safety
    messages_.emplace_back(text);
    storage_.save_messages(messages_); // Save after adding
}

std::vector<Message> ChatManager::get_messages() const {
     // No lock needed for const access if reads are safe during writes (vector push_back might reallocate)
     // For simplicity here, assume reads are okay or lock if strict safety needed:
     // std::lock_guard<std::mutex> lock(mutex_);
    return messages_;
}

// Helper to format messages as a JSON array string for the API
std::string ChatManager::get_messages_as_json_array() const {
    // std::lock_guard<std::mutex> lock(mutex_); // Lock if needed
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < messages_.size(); ++i) {
        ss << messages_[i].serialize();
        if (i < messages_.size() - 1) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}
