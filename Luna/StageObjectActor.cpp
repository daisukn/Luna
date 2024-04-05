#include "StageObjectActor.h"

StageObjectActor::StageObjectActor(Application* a, class Stage* s)
    : Actor(a)
    , isDisp(false)
    , state(StateNormal)
    , cntLifetime(0)
    , ownerStage(s)
{
    s->AddStageActor(this);
}

void StageObjectActor::UpdateActor(float deltaTime)
{
    
}

void StageObjectActor::Appear(Vector3 pos, int type)
{
    cntLifetime = 0;
    behaveType = type;
    state = StateNormal;
    isDisp = true;
    SetRotation(Quaternion::Identity);
    SetPosition(pos);
}

void StageObjectActor::Disappear()
{
    isDisp = false;
}
