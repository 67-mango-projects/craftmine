#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <nlohmann/json.hpp>

class SaveSystem {
public:
    /**
     * Constructor that initializes the SaveSystem with the specified file path.
     * @param path Path to the save file.
     */
    explicit SaveSystem(std::filesystem::path path);

    /**
     * Saves a string value under the specified key.
     * @param key Key for the value to be saved.
     * @param value Value to be saved.
     * @return bool containing if the save was successful.
     */
    bool save(const std::string& key, const std::string& value);

    /**
     * Reads a string value associated with the specified key.
     * @param key value key to be read.
     * @return std::nullopt if the key doesn't exist or isn't a string, otherwise the string value.
     */
    [[nodiscard]] std::optional<std::string> get(const std::string& key) const;

private:
    std::filesystem::path path_;
    nlohmann::json data_ = nlohmann::json::object();

    /**
     * Loads the saved file (or backup) into `data_`.
     * @return bool containing if the load was successful.
     */
    bool load();

    /**
     * Persists `data_` to disk safely (temporary file + backup).
     * @return bool containing if the persist was successful.
     */
    bool persist();
};
