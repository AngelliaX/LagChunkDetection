#include "pch.h"
#include"loader/Loader.h"
#include "Header.h"
size_t totalRound = 100;
size_t currentRound = 0;
THook(void, "?tick@LevelChunk@@QEAAXAEAVBlockSource@@AEBUTick@@@Z", void* levelChunk, class BlockSource* blockSource, size_t* tick) {
    //std::cout << levelChunk <<"..."<<blockSource<<"..."<<tick << "\n";
    TIMER_START
        original(levelChunk, blockSource, tick);
    TIMER_END
        auto rounds = static_cast<float>(totalRound * 1000);
    chunkTickTime += timeReslut;
    //std::cout << chunkTickTime / rounds << "\n";
    //std::cout << "end====================\n\n";
}

std::unordered_map<string, int> playersDim;
std::unordered_map<string, Vec3> playersVec3;
int maxFar = 128;//128 block
Minecraft* mc;
Vec3 spawnPoint;
void onProgressing() {
    for (Player* pl : liteloader::getAllPlayers())
    {
        WPlayer player = WPlayer{ *pl };
        if(false){ 
            //execute when there is no laggy chunk
            return;
        }
        //execute when a laggy chunk is detected
        if (playersDim.find(player.getName())->first == "") {
            //Call when the player is not inside any map (new log in)
            //teleport to spawn
        }
        //check dimension
        if (player.getDimID() != playersDim.find(player.getName())->second) {
            //teleport to spawn
        }
        int dim = player.getDimID();
        std::cout << dim;

        //player.teleport(spawnPoint, dim);
        player.teleport(pl->getPos(), dim);
        return;
        //compare coord;
        bool checkX = abs(pl->getPos().x - playersVec3.find(player.getName())->second.x) > maxFar;
        bool checkY = abs(pl->getPos().y - playersVec3.find(player.getName())->second.y) > maxFar;
        if (checkX or checkY) {
            //teleport to spawn
        }
    }
}
bool oncmd_wow(CommandOrigin const& ori, CommandOutput& outp, string const& str, optional<int>& str2) {
    outp.addMessage(str);
    onProgressing();
    return true;
}
void entry() {
    Level* level = mc->getLevel();
    LevelData* levelData = SymCall("?getLevelData@Level@@UEBAAEBVLevelData@@XZ", LevelData*, Level*)(level);
    spawnPoint.x = levelData->getSpawnPos().x;
    spawnPoint.y = levelData->getSpawnPos().y;
    spawnPoint.z = levelData->getSpawnPos().z;


    std::cout << "Hello World\n";
    Event::addEventListener([](RegCmdEV ev) {
        CMDREG::SetCommandRegistry(ev.CMDRg);
        MakeCommand("wow", "awesome", 0);//注册指令
        CmdOverload(wow, oncmd_wow, "input", "optinal");//重载指令
        });
}
