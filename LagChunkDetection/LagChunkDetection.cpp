#include "pch.h"
#include "GlobalVar.h"

#include "loader/Loader.h"
#include "ChunkTickHeader.h"
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
Minecraft* mc;

void onProgressing() { //called after you have the chunkTick value
    for (Player* pl : liteloader::getAllPlayers())
    {
        WPlayer player = WPlayer{ *pl };   
        if (false) { //chunktick value comparision
            //execute when there is no laggy chunk
            return;
        }
        bool checkX = abs(pl->getPos().x - playersVec3.find(player.getRealName())->second.x) > maxFar;
        bool checkY = abs(pl->getPos().y - playersVec3.find(player.getRealName())->second.y) > maxFar;
        int dim = player.getDimID();
        Vec3 pos;
        pos.x = 0;
        pos.y = 200;
        pos.z = 0;
        
        //execute when a laggy chunk is detected
        if (playersDim.find(player.getRealName()) == playersDim.end()) {
            std::cout << "Player not found" << "\n";
            player.teleport(pos, dim);
            goto end;
        }  
        //check dimension
        dim = playersDim.find(player.getRealName())->second;
        if (player.getDimID() != dim) {
            std::cout << "Dim is not corresponding" << "\n";
            player.teleport(pos, dim);
            goto end;
        }
        //compare coord;
         if (checkX or checkY) {
             player.sendText("You may be causing some lag, teleporting to spawn...");
             std::cout << "Teleport player" << "\n";
             player.teleport(pos, dim);
        }
        end:
        std::cout << player.getRealName() << "\n";
        playersDim[player.getRealName()] = dim;
        playersVec3[player.getRealName()] = pl->getPos();
        std::cout << "added to var \n";
        return;
        
    }
}
bool onCMD(CommandOrigin const& ori, CommandOutput& outp, optional<int>& str, optional<int>& str2) {
    onProgressing();
    return true;
}

void getSpawnPoint() {
    /*Level* level = mc->getLevel();
    LevelData* levelData = SymCall("?getLevelData@Level@@UEBAAEBVLevelData@@XZ", LevelData*, Level*)(level);
    levelData->getSpawnPos();*/
}
void loadConfig();
void entry() {
    loadConfig();
    getSpawnPoint();
    Event::addEventListener([](RegCmdEV ev) {
        CMDREG::SetCommandRegistry(ev.CMDRg);
        MakeCommand("lagchunkdetection", "awesome", 0);//注册指令
        CmdOverload(lagchunkdetection, onCMD, "input", "optinal");//重载指令
    });
}