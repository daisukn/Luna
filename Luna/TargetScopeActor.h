#pragma once

#include "StageObjectActor.h"
#include "BillboardComponent.h"
#include <memory>


class TargetScopeActor : public StageObjectActor
{
public:
    TargetScopeActor(class Application* a, class Stage* s);
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class BillboardComponent> mCcopeBillboard;
};

