#include "storage.h"
#include <fstream>
#include <iostream>
#include <vector>

Storage::Storage(const std::string &path) : storage_path_(path) {}

std::vector<Message> Storage::load_messages()
{
    std::vector<Message> messages;
    std::ifstream file(storage_path_);
    std::string line;

    if (!file.is_open())
    {

        return messages;
    }

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            try
            {
                messages.push_back(Message::deserialize(line));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error deserializing line: " << line << " - " << e.what() << std::endl;
            }
        }
    }
    file.close();
    return messages;
}

void Storage::save_messages(const std::vector<Message> &messages)
{
    std::ofstream file(storage_path_, std::ios::trunc);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open storage file for writing: " << storage_path_ << std::endl;
        return;
    }

    for (std::vector<Message>::const_iterator it = messages.begin(); it != messages.end(); ++it)
    {
        const Message &msg = *it;
        file << msg.serialize() << std::endl;
    }
    file.close();
}
