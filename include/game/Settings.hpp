#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>
#include <string>

enum GameMode
{
    ARCADE,
    SPEED_CHALLENGE,
    TOURNAMENT,
    HEAD_TO_HEAD
};



namespace settings
{
    // Load & Save
    void Load();
    void Save();


    //Get and Set options from the optionmenu
    bool IsMusicOn();
    int  GetMusicVolume();

    void EnableMusic(bool value);
    void AdjustVolume(int value);

    void SetAntialiasing(unsigned int value);
    unsigned int GetAntialiasing();

    void SetTimeForTracer(int milliseconds);
    int GetTimeForTracer();

    //Settings from the current level
    void SetGamemode(const GameMode& gamemode);
    GameMode GetGamemode();

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

#endif // SETTINGS_HPP
