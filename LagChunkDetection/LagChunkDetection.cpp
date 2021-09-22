#include "pch.h"
#include "GlobalVar.h"
#include "ChunkTickHeader.h"


std::unordered_map<string, int> playersDim;
std::unordered_map<string, Vec3> playersVec3;
Minecraft* mc;
void onProgressing() { //called after you have the chunkTick value
    int teleportedNum = 0;
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
            player.teleport(pos, spawnDim);
            teleportedNum++;
            goto end;
        }  
        //check dimension
        dim = playersDim.find(player.getRealName())->second;
        if (player.getDimID() != dim) {
            player.sendText(warn_message);
            player.teleport(pos, spawnDim);
            teleportedNum++;
            goto end;
        }
        //compare coord;
        if (checkX or checkY) {
            player.sendText(warn_message);
            player.teleport(pos, spawnDim);
            teleportedNum++;
        }    
        end:
        playersDim[player.getRealName()] = dim;
        playersVec3[player.getRealName()] = pl->getPos();     
    }
    if (chunkTickResult >= maxChunkTick) { //chunktick value comparision
        if (teleportedNum == 0) {
            Vec3 pos;
            pos.x = spawnPos[0];
            pos.y = spawnPos[1];
            pos.z = spawnPos[2];
            for (Player* pl : liteloader::getAllPlayers()) {
                WPlayer player = WPlayer{ *pl };
                player.sendText(warn_message);
                player.teleport(pos, spawnDim);
            }
        }
    }
}
bool onCMD(CommandOrigin const& ori, CommandOutput& outp) {
    WPlayer player = MakeWP(ori).val();
    player.sendText("§c==================");
    player.sendText("§bLag§6Chunk§aDetection §fversion 1.0");
    player.sendText("§fmade by Tungstenvn aka Hello你好");
    player.sendText("§c==================");
    return true;
}
bool admin_Command(CommandOrigin const& ori, CommandOutput& outp, string const& str, int const& str2) {
    WPlayer player = MakeWP(ori).val();
    if (str == "maxchunktick") {
        maxChunkTick = str2;
        player.sendText("§aChange maxChunkTick value to §c" + std::to_string(str2) + "§a successfully");
    }
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
        MakeCommand("lcd", "§aDetect whether there is any lag chunk or not", 0);
        CmdOverload(lcd, onCMD);
        MakeCommand("lcdset", "§cAdmin only", 1);
        CmdOverload(lcdset, admin_Command, "category", "value");
    });
    std::cout << "  _                  _____ _                 _    \n | |                / ____| |               | |   \n | |     __ _  __ _| |    | |__  _   _ _ __ | | __\n| |    / _` |/ _` | |    | '_ \| | | | '_ \| |/ /\n | |___| (_| | (_| | |____| | | | |_| | | | |   < \n |______\__,_|\__, |\_____|_| |_|\__,_|_| |_|_|\_\\\n               __/ |                              \n              |___/                               \n";
    std::cout << "                         made by Hello你好 x People\n";
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
            isFinishChecking = false;
            std::cout << "==========ChunkTick value: " << chunkTickResult << " ms\n";
            onProgressing();
        }
    }
    else {
        //std::cout << "====\n";
    }
}
size_t totalRound = 100;
size_t currentRound = 0;

THook(void, "?tick@ServerLevel@@UEAAXXZ", class Level* serverLevel) {
    original(serverLevel);
    if (isCheck) {
        currentRound++;
    }
}
THook(void, "?tick@LevelChunk@@QEAAXAEAVBlockSource@@AEBUTick@@@Z", void* levelChunk, class BlockSource* blockSource, size_t* tick) {
    if (isCheck) {
        TIMER_START
            original(levelChunk, blockSource, tick);
        TIMER_END
            auto rounds = static_cast<float>(totalRound*1000);
        chunkTickTime += timeReslut; 
        if (currentRound >= totalRound) {
            chunkTickResult = round(chunkTickTime / rounds);
            std::cout << chunkTickTime / rounds << "ms\n";
            currentRound = 0;
            chunkTickTime = 0;        
            isCheck = false;
            isFinishChecking = true;
        }
    }
    else {
        original(levelChunk, blockSource, tick);
    }
}
