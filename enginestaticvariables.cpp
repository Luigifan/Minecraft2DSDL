/**
  About {{filename}}.h/{{filename}}.cpp
  By: Mike

*/

#include "enginestaticvariables.h"
#include "Camera2d.h"
#include "SoundMixer.h"
#include "Tile.h"
#include "ScreenManager.h"
#include "LevelBackground.h"
#include "src/keyboardmonitor.h"

#include <SDL.h>

int EngineStaticVariables::TOTAL_TILE_COUNT = 3;
int EngineStaticVariables::TOTAL_BACKGROUND_COUNT = 1;
int EngineStaticVariables::InternalWidth = 800;
int EngineStaticVariables::InternalHeight = 600;

Tile* EngineStaticVariables::airTile = nullptr;

bool EngineStaticVariables::DoQuit = false;
bool EngineStaticVariables::UpdateGame = true;

const int EngineStaticVariables::TargetFramerate = 60;

const Uint8* EngineStaticVariables::currentKeystate;
std::map<int, Tile*> EngineStaticVariables::Tilemap;
std::map<int, LevelBackground*> EngineStaticVariables::BackgroundMap;

SoundMixer* EngineStaticVariables::MainSoundMixer = new SoundMixer(EngineStaticVariables::GetResourcesPath());
Camera2d* EngineStaticVariables::MainGameCamera = new Camera2d(0, 0);
ScreenManager* EngineStaticVariables::MainScreenManager = new ScreenManager();
InputMonitor* EngineStaticVariables::MainKeyboardInputWatcher = new InputMonitor();

//methods
Tile* EngineStaticVariables::GetBlockByID(int id)
{
    if(id <= 0)
    {
        if(EngineStaticVariables::airTile == nullptr)
        {
            airTile = new Tile();
            airTile->setBlockName("Air");
            airTile->setBlockSize(32, 32);
        }
        return airTile;
    }
    return Tilemap[id];
}

LevelBackground* EngineStaticVariables::GetBackgroundByID(int id)
{
    return BackgroundMap[id];
}

std::string EngineStaticVariables::GetResourcesPath()
{
    try
    {
        std::string returnValue = SDL_GetBasePath() + std::string("/res");
        return returnValue;
    }
    catch(std::exception &what)
    {
        std::cerr << what.what() << std::endl;
    }
    return "";
}
