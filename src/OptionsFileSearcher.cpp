#include "OptionsFileSearcher.hpp"
#include "amethyst/Log.hpp"
#include <fstream>
#include <iostream>
#include <string>

std::vector<int> findExistingKeys(std::string_view actionName, const std::vector<int>& keysIfNotFound)
{
    const std::filesystem::path optionsPath{ GetComMojangPath() / L"minecraftpe" / L"options.txt" };
    std::ifstream inf{ optionsPath };

    if (!inf)
    {
        Log::Info("options.txt failed to open, returning default {} keys.", actionName);
        return keysIfNotFound;
    }

    // Parse the file while there are lines left, and until the line contains "keyboard_type_0_{actionName}:"
    std::string line{};
    const std::string desiredLineName{ "keyboard_type_0_" + static_cast<std::string>(actionName) + ":" };
    while (std::getline(inf, line))
    {
        if (line.find(desiredLineName) != std::string::npos)
        {
            std::string_view keysView{ line };
            keysView.remove_prefix(34); // The keybinds start at character 34

            // Iterate through the keys and add them to a vector
            std::vector<int> keys{};
            size_t commaPosition{ keysView.find_first_of(',') };
            while (commaPosition != std::string::npos)
            {
                std::string key{ keysView.substr(0, commaPosition) };
                if (key.compare("0") != 0) // 0 in options.txt means "Unassigned"
                {
                    Log::Info("Found existing {} key: {}", actionName, key);
                    keys.emplace_back(std::stoi(key));
                }

                keysView.remove_prefix(commaPosition + 1);
                commaPosition = keysView.find_first_of(',');
            }
            // No more commas means there is either one more key, or no keys.
            if (!keysView.empty())
            {
                std::string lastKey{ keysView };
                if (lastKey.compare("0") != 0) // 0 in options.txt means "Unassigned"
                {
                    Log::Info("Found last existing {} key: {}", actionName, lastKey);
                    keys.emplace_back(std::stoi(lastKey));
                }
            }

            if (!keys.empty())
                return keys;
        }
    }

    Log::Info("No existing keybinds found for {}, returning default keys.", actionName);
    return keysIfNotFound;
}