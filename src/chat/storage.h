#ifndef STORAGE_H
#define STORAGE_H

#include "message.h"
#include <vector>
#include <string>

class Storage {
private:
    std::string storage_path_;

public:
    Storage(const std::string& path);
    std::vector<Message> load_messages();
    void save_messages(const std::vector<Message>& messages);
};

#endif // STORAGE_H
