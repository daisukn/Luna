#pragma once

#include "Actor.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "ColliderComponent.h"
#include "Stage.h"
#include <memory>

enum ObjectState
{
    StateNormal,
    StateExploted
};

// ゲームオブジェクトの基底クラス
class StageObjectActor : public Actor
{
public:
    StageObjectActor(class Application* a, class Stage* s);

    void UpdateActor(float deltaTime) override;

    //void SetVisible(bool visible);
    //bool GetVisible() const { return isVisible; }
    
    bool GetDisp() const { return isDisp; }
    void SetDisp(bool b) { isDisp = b; }
    
    void SetOwnerStage(class Stage* s) { ownerStage = s; }
    
    virtual void Appear(Vector3 pos, int type);
    virtual void Disappear();
    
    void SetBehaveType(int t) { behaveType = t; }
    int GetBehaveType() const { return behaveType; }
    
    void SetPlayerPosition(Vector3 v) { playerPos = v; }
    
protected:
    std::unique_ptr<class MeshComponent> meshComp;
    std::unique_ptr<class ColliderComponent> collComp;
    bool isDisp;

    ObjectState state;
    
    
    int behaveType;
    
    // カウンター
    int cntLifetime;
    
    Vector3 playerPos;
    
    // オーナーのステージ
    class Stage* ownerStage;
};

