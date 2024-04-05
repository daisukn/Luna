#pragma once

#include "StageObjectActor.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "ColliderComponent.h"
#include "TargetScopeActor.h"
#include "LaserActor.h"
#include "SpriteComponent.h"
#include "Actor.h"

#include <memory>


// 飛行機
class PlaneActor : public StageObjectActor
{
public:
    PlaneActor(class Application* app, class Stage* s);

    void ActorInput(const struct InputState& state) override;
    void UpdateActor(float deltaTime) override;

    void SetMeshVisible(bool visible);
    
    int GetLife() const { return mLife; }
    int GetMaxLife() const { return mMaxLife; }
    
    
private:
   
    // モーションID
    int mAnimID;
    
    //std::unique_ptr<class SkeletalMeshComponent> skeltalMeshComp;
    std::unique_ptr<class MoveComponent> mMoveComp;
    std::unique_ptr<class FollowCamera> mCameraComp;
    std::unique_ptr<class MeshComponent> mLightning;
    
    std::unique_ptr<class TargetScopeActor> mScopeActor;
    

    // フィールド移動用
    void FieldMove(const struct InputState& state);

    
    // 動作可能=true 動作不可能=false
    bool mIsMovable;
    
    void ShotLaser();
    std::vector<std::unique_ptr<class StageObjectActor>> laserActor;
    
    int mBarrierCnt;
    void DamageEffect(bool b);
    
    int mLife;
    int mMaxLife;

};

