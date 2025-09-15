#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <spdlog.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

class SaveSystem {
public:
    explicit SaveSystem(fs::path path) : path_(std::move(path)) {
        load();
    }

    bool save(const std::string& key, const std::string& value) {
        data_[key] = value;
        return persist();
    }

    // Read value
    [[nodiscard]] std::optional<std::string> get(const std::string& key) const {
        auto it = data_.find(key);
        if (it != data_.end() && it->is_string()) {
            return it->get<std::string>();
        }
        return std::nullopt;
    }

private:
    fs::path path_;
    json data_ = json::object();

    bool load() {
        try {
            if (fs::exists(path_)) {
                std::ifstream in(path_);
                data_ = json::parse(in, /*callback*/nullptr, /*allow_exceptions*/true);
                return true;
            }
            // If it don't exist, try to load backup
            auto bak = path_;
            bak += ".bak";
            if (fs::exists(bak)) {
                std::ifstream in(bak);
                data_ = json::parse(in, nullptr, true);
                return true;
            }
        } catch (const std::exception& e) {
            spdlog::debug("[SaveSystem] Failed to load: {}", e.what());
        }
        data_ = json::object();
        return false;
    }

    bool persist() {
        try {
            // Make shure the directory exists
            if (!path_.parent_path().empty()) {
                fs::create_directories(path_.parent_path());
            }

            const auto tmp = path_.string() + ".new";
            const auto bak = path_.string() + ".bak";

            // Serializes the JSON with indentation
            const std::string serialized = data_.dump(2);

            // Writes to a temporary file first
            {
                std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
                if (!out) throw std::runtime_error("Can't open temporary file for writing");
                out.write(serialized.data(), static_cast<std::streamsize>(serialized.size()));
                out.flush(); // Makes shure that all data is written to disk
                if (!out) throw std::runtime_error("Failed to write temporary file for writing");
            }

            // Makes a backup of the current save, if it exists
            std::error_code ec;
            if (fs::exists(path_)) {
                fs::remove(bak, ec); // remove old .bak, if exists
                fs::rename(path_, bak, ec); // renames current save to .bak
                // if any error occurs, we ignore it cuz they are not critical
            }

            // Move atomic to new location
            fs::rename(tmp, path_);

            return true;
        } catch (const std::exception& e) {
            spdlog::debug("[SaveSystem] Failed to persist: {}", e.what());
            return false;
        }
    }
};

// how to use it:
/*
    // The final file will be in "saves/save.json"
    // Create a new class instance
    SaveSystem db(fs::path("saves") / "save.json");

    // Save smt: save(key, value) - both strings.
    db.save("player_name", "Po");
    db.save("dimension", "1");
    db.save("cords", "150");
    db.save("next_dimension_unlocked", "true");

    // Read a value
    if (auto dimension = db.get("dimension")) {
        std::cout << "Gold atual: " << *gold << "\n";
    }
*/