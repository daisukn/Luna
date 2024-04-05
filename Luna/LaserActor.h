#pragma once

#include "StageObjectActor.h"
#include "ParticleComponent.h"

class LaserActor : public StageObjectActor
{
public:
    LaserActor(class Application* a, class Stage* s);
    void UpdateActor(float deltaTime) override;
    virtual void Appear(Vector3 pos, int type) override;
    virtual void Disappear() override;
private:
    std::unique_ptr<class ParticleComponent> mParticle;
    std::unique_ptr<class MeshComponent> mEffectMesh;
};


