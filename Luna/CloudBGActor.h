#pragma once

#include "Actor.h"

#include "BillboardComponent.h"
#include "MoveComponent.h"
#include <memory>

// 背景用雲
class CloudBGActor : public Actor
{
public:
    CloudBGActor(class Application* a);

    void UpdateActor(float deltaTime) override;

    //void SetVisible(bool visible);
    //bool GetVisible() const { return isVisible; }
    
    bool GetDisp() const { return mIsDisp; }
    void SetDisp(bool b) { mIsDisp = b; }
    
private:
    std::unique_ptr<class BillboardComponent> mCloudBillboard;
    bool mIsDisp;

};
