#include "pch.h"
#include "GlobalVar.h"
#include "ChunkTickHeader.h"


std::unordered_map<string, int> playersDim;
std::unordered_map<string, Vec3> playersVec3;
Minecraft* mc;
void onProgressing() { //called after you have the chunkTick value
    for (Player* pl : liteloader::getAllPlayers())
    {
        WPlayer player = WPlayer{ *pl };   
        bool checkX = abs(pl->getPos().x - playersVec3.find(player.getRealName())->second.x) > maxFar;
        bool checkY = abs(pl->getPos().y - playersVec3.find(player.getRealName())->second.y) > maxFar;
        int dim = player.getDimID();

        if (chunkTickResult < maxChunkTick) { //chunktick value comparision
            goto end;
        }
        Vec3 pos;
        pos.x = spawnPos[0];
        pos.y = spawnPos[1];
        pos.z = spawnPos[2];
        //check if the variable contains current player
        if (playersDim.find(player.getRealName()) == playersDim.end()) {
            player.sendText(warn_message);          
            player.teleport(pos, dim);
            goto end;
        }  
        //check dimension
        dim = playersDim.find(player.getRealName())->second;
        if (player.getDimID() != dim) {
            player.sendText(warn_message);
            std::cout << "Dim is not corresponding" << "\n";
            player.teleport(pos, dim);
            goto end;
        }
        //compare coord;
        if (checkX or checkY) {
            player.sendText(warn_message);;
            std::cout << "Teleport player" << "\n";
            player.teleport(pos, dim);
        }
        end:
        playersDim[player.getRealName()] = dim;
        playersVec3[player.getRealName()] = pl->getPos();
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
        MakeCommand("lcd", "Detect whether there is any lag chunk or not", 0);//注册指令
        CmdOverload(lcd, onCMD, "input", "optinal");//重载指令
    });
}
int timePass = 0;
bool isCheck = false;
bool isFinishChecking;
THook(void, "?tick@Level@@UEAAXXZ", class Level* lv) {
    original(lv);
    timePass++;
    if (timePass >= checkTime_inTick) {
        if (!isCheck) {
            isCheck = true;
        }
        if (isFinishChecking) {
            timePass = 0;
            isCheck = false;
            isFinishChecking = false;
            std::cout << "ChunkTick value: " << chunkTickResult << " ms\n";
            onProgressing();
        }
    }
    else {
        std::cout << "====\n";
    }
}
size_t totalRound = 100;
size_t currentRound = 0;
THook(void, "?tick@LevelChunk@@QEAAXAEAVBlockSource@@AEBUTick@@@Z", void* levelChunk, class BlockSource* blockSource, size_t* tick) {
    if (isCheck) {
        TIMER_START
            original(levelChunk, blockSource, tick);
        TIMER_END
            auto rounds = static_cast<float>(1000);
        chunkTickTime += timeReslut;
        currentRound++;
        if (currentRound >= totalRound) {
            chunkTickResult = round(chunkTickTime / rounds);
            std::cout << chunkTickTime / rounds << "ms\n";
            currentRound = 0;
            chunkTickTime = 0;
            isFinishChecking = true;
        }
    }
    else {
        original(levelChunk, blockSource, tick);
    }

}
