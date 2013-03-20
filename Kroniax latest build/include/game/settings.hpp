
#include <vector>
#include <string>

#include <random>
#include <time.h>

#include "DialogScreen.hpp"

namespace settings
{


    void Load();
    void Save();

    bool IsMusicOn();
    int  GetMusicVolume();

    void EnableMusic(bool value);
    void AdjustVolume(int value);

    void SetGamemode(int value);
    int GetGamemode();

    std::vector<std::string> &GetLevelList();
    int GetUnlockedLevel();

    bool IncreaseUnlockLevel();

    float GetSpeedX();
    float GetSpeedY();

    void SetSpeedX(float value);
    void SetSpeedY(float value);

    void SetStartSpeedX(float value);
    void SetDownforce(float value);

    void SetStartDownforce(float value);

    float GetStartSpeedX();
    float GetDownforce();
    float GetStartDownforce();

    void SetMusic(int index);
    int GetMusic();

}
