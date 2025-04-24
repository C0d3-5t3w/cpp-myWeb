#include "storage.h"
#include <fstream>
#include <iostream> // For error logging
#include <vector> // Ensure vector is included for iterator types

Storage::Storage(const std::string& path) : storage_path_(path) {}

std::vector<Message> Storage::load_messages() {
    std::vector<Message> messages;
    std::ifstream file(storage_path_);
    std::string line;

    if (!file.is_open()) {
        // If the file doesn't exist, it's fine, just return empty vector.
        // Log if it's another error? For now, just proceed.
        // std::cerr << "Warning: Could not open storage file for reading: " << storage_path_ << std::endl;
        return messages;
    }

    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                 messages.push_back(Message::deserialize(line));
            } catch (const std::exception& e) {
                std::cerr << "Error deserializing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }
    file.close();
    return messages;
}

void Storage::save_messages(const std::vector<Message>& messages) {
    std::ofstream file(storage_path_, std::ios::trunc); // Overwrite existing file
    if (!file.is_open()) {
        std::cerr << "Error: Could not open storage file for writing: " << storage_path_ << std::endl;
        return;
    }

    for (std::vector<Message>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
        const Message& msg = *it; // Get reference to the message
        file << msg.serialize() << std::endl;
    }
    file.close();
}
