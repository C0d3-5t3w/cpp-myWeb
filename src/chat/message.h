#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

struct Message {
    std::string text;
    std::string timestamp;

    Message(std::string txt) : text(std::move(txt)) {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        timestamp = ss.str();
    }

    // Constructor for loading from storage
    Message(std::string txt, std::string ts) : text(std::move(txt)), timestamp(std::move(ts)) {}

    // Simple serialization for storage/API (not robust JSON)
    std::string serialize() const {
        // Replace quotes to avoid breaking simple parsing
        std::string safe_text = text;
        size_t pos = safe_text.find('\"');
        while(pos != std::string::npos) {
            safe_text.replace(pos, 1, "\\\"");
            pos = safe_text.find('\"', pos + 2);
        }
        return "{\"timestamp\": \"" + timestamp + "\", \"text\": \"" + safe_text + "\"}";
    }

    // Simple deserialization (assumes specific format)
    static Message deserialize(const std::string& line) {
        std::string ts, txt;
        size_t ts_start = line.find("\"timestamp\": \"");
        size_t ts_end = line.find("\"", ts_start + 14);
        if (ts_start != std::string::npos && ts_end != std::string::npos) {
            ts = line.substr(ts_start + 14, ts_end - (ts_start + 14));
        }

        size_t txt_start = line.find("\"text\": \"");
        size_t txt_end = line.find("\"}", txt_start + 9); // Look for closing "}
        if (txt_start != std::string::npos && txt_end != std::string::npos) {
            txt = line.substr(txt_start + 9, txt_end - (txt_start + 9));
            // Unescape quotes
             size_t pos = txt.find("\\\"");
            while(pos != std::string::npos) {
                txt.replace(pos, 2, "\"");
                pos = txt.find("\\\"", pos + 1);
            }
        }
        return Message(txt, ts);
    }
};

#endif // MESSAGE_H
