#pragma once

#include "Stage.h"
#include "SpriteComponent.h"
#include <memory>


class TitleStage : public Stage
{
private:
    std::unique_ptr<class Actor> mMoonActor;
    std::unique_ptr<class Actor> mPlaneActor;
    std::unique_ptr<class Actor> mSkyActor;
    std::unique_ptr<class MeshComponent> mMoonMesh;
    std::unique_ptr<class SkeletalMeshComponent> mPlaneMesh;
    std::unique_ptr<class SpriteComponent> mSkySprite;
public:
    TitleStage(class Application* a);
    virtual ~TitleStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;
};

