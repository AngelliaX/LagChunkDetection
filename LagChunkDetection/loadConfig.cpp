#include "pch.h"
#include "GlobalVar.h"

#include <filesystem>
#include <rapidjson/document.h>
int maxFar;
std::vector<int> spawnPos;
int maxChunkTick;
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
        std::cout << "[LagChunkDetection] " << config_file << " not found, creating file(default value used)\n";
        std::ofstream of(config_file);
        if (of)
        {
            std::string text = std::string("{\n  \"maxFar\": \"") + std::to_string(maxFar) + "\",\n  \"maxChunkTick\": \""+std::to_string(maxChunkTick)+"\",\n  \"spawnPos\": \"[0,100,0]\"" + "\n }";
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
        maxFar = document["maxFar"].GetInt();
        maxChunkTick = document["maxChunkTick"].GetInt();
        auto arraylist = document["spawnPos"].GetArray();
        for (rapidjson::Value::ConstValueIterator itr = arraylist.Begin(); itr != arraylist.End(); ++itr) {
            spawnPos.push_back(itr->GetInt());
        }
    }
}

