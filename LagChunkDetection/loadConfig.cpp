#include "pch.h"
#include "GlobalVar.h"

#include <filesystem>
#include <rapidjson/document.h>
int maxFar;
std::vector<int> spawnPos;
int maxChunkTick;
int checkTime_inTick;
int spawnDim;
string warn_message;
void loadConfig() {
    std::filesystem::create_directory("plugins\\LagChunkDetection");
    std::string config_file = "plugins\\LagChunkDetection\\config.json";
    std::ifstream fs;
    fs.open(config_file, std::ios::in);
    if (!fs)
    {
        maxFar = 128;
        maxChunkTick = 60;
        spawnPos = { 0,100,0 };
        checkTime_inTick = 600;
        spawnDim = 0;
        warn_message = "There may be some lag chunk near you, teleporting to spawn...";
        std::cout << "[LagChunkDetection] " << config_file << " not found, creating file(default value used)\n";
        std::ofstream of(config_file);
        if (of)
        {
            std::string text = std::string("{\n  \"maxFar\": \"") + std::to_string(maxFar) + "\",\n  \"maxChunkTick\": \""+std::to_string(maxChunkTick) + "\",\n  \"checkTime_inTick\": \"" + std::to_string(checkTime_inTick)+"\",\n  \"spawnPos\": [\"10064\",\"69\",\"10029\"]" + ",\n  \"spawnDim\": \"" + std::to_string(spawnDim) + "\",\n  \"warn_message\": \"" + warn_message + "\"\n}";
            of << text;
        }
        else
        {
            std::cout << "[LagChunkDetection] " << "config file creation failed, please create \"config\" folder\n";
        }
    }
    else
    {
        std::string json;
        char buf[1024];
        while (fs.getline(buf, 1024))
        {
            json.append(buf);
        }
        rapidjson::Document document;
        document.Parse(json.c_str());
        maxFar = std::stoi(document["maxFar"].GetString());
        maxChunkTick = std::stoi(document["maxChunkTick"].GetString());
        checkTime_inTick = std::stoi(document["checkTime_inTick"].GetString());
        warn_message = document["maxFar"].GetString();
        spawnDim = std::stoi(document["spawnDim"].GetString());
        auto arraylist = document["spawnPos"].GetArray();
        for (rapidjson::Value::ConstValueIterator itr = arraylist.Begin(); itr != arraylist.End(); ++itr) {
            spawnPos.push_back(std::stoi(itr->GetString()));
        }
    }
}

