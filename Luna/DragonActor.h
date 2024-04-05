#pragma once

#include "Actor.h"
#include <memory>


// 飛行機
class DragonActor : public Actor
{
public:
    DragonActor(class Application* app);
    void UpdateActor(float deltaTime) override;
    void SetVisible(bool visible);
    
    

private:
    //  前フレームの場所
    //Vector3 mPrevPos;
    
    // モーションID
    int mAnimID;
    
    std::unique_ptr<class SkeletalMeshComponent> mMeshComp;
    
    // モーションを適用
    void ApplyMotion();
    


};

