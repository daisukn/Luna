#pragma once

#include "Stage.h"
#include "UfoEnemy.h"
#include "MoaiEnemy.h"
#include "UIElement.h"
#include <memory>
#include <vector>


class CloudStage : public Stage
{
public:
    CloudStage(class Application* a);
    ~CloudStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;

    
    
private:
    std::unique_ptr<class PlaneActor> mPlaneActor;
    std::unique_ptr<class DragonActor> mDragonActor;

    std::vector<std::unique_ptr<class CloudBGActor>> mCloudActor;
    std::vector<std::unique_ptr<class StageObjectActor>> mUfoEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> mMoaiEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> mShipEnemy;

    

    std::unique_ptr<class Actor> mSkyActor;
    std::unique_ptr<class MeshComponent> mSkyMesh;
    
    std::unique_ptr<class UIElement> mUiElement;
    
    //float mAngle = 0.f;
    
    unsigned int mStageCounter;
    
    void GenerateCloud();
    void AppearLayout() override;
    void AppearRandom();
    void DebugAppear();


        
};

