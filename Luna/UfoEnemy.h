#pragma once

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include "BulletActor.h"
#include <memory>
#include <vector>

class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a, class Stage* s);
    ~UfoEnemy();
    void UpdateActor(float deltaTime) override;
    void Appear(Vector3 pos, int type) override;
    void Disappear() override;
private:

    std::unique_ptr<class ExplosionActor> mExplosion;
    std::vector<std::unique_ptr<class BulletActor>> mBullets;
    std::unique_ptr<class MoveComponent> mMoveComp;

    void CheckCllider();
        
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (UfoEnemy::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
    
    void ShotCircle();
    void ShotLiner();

    
    
    float mForwardSpeed;
    float mAnglerSpeed;
    float mUpperSpeed;
    
    float mAngle;
    
};

