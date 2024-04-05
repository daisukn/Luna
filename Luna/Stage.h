#pragma once

#include "Math.h"
#include <memory>
#include <vector>
#include <string>

struct GameStatus
{
    int playerLife;
    int playerScore;
    Vector3 playerPosition;
};


struct StageLayout
{
    int frame;
    int objType;
    int behaveType;
    float x;
    float y;
    float z;
};


class Stage
{
public:
    Stage(class Application* a);
    virtual ~Stage();
    
    virtual void LoadStageData();
    virtual void UpdateStage();
    virtual void UnloadStageData();

    void ProcessInput(const struct InputState& state);
    // 派生先での入力処理 Override
    virtual void StageInput(const struct InputState& state);
    
    bool GetQuitStage() const { return mIsQuitStage; }
    
    void AddStageActor(class StageObjectActor* a);
    
    Vector3 GetPlayerPosition() const { return mPlayerPos;}
    int GetPlayerLife() { return mPlayerLife; }
    
    
protected:
    class Application* mApp;
    
    bool mIsQuitStage; // trueになったら遷移する
    
    std::vector<struct StageLayout> mLayout;
    void LoadStageLayout(std::string filename);
    virtual void AppearLayout();

    int mCntLayout;
    int mNumLayout;
    
    GameStatus mGameStatus;
    
    std::vector<class StageObjectActor*> mStageActors;
    
    Vector3 mPlayerPos;
    int mPlayerLife;
    
private:
};

