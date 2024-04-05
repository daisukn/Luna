#include "LaserActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

LaserActor::LaserActor(Application* a, Stage* s)
    : StageObjectActor(a, s)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/laser.lwo"));
    meshComp->SetVisible(false);
    //meshComp->SetToonRender(true, 1.04f);
    
    SetScale(0.05f);
    
    mEffectMesh = std::make_unique<MeshComponent>(this, false, MESH_EFFECT);
    mEffectMesh->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/stripe.lwo"));
    mEffectMesh->SetVisible(false);
    mEffectMesh->SetScale(1.05f);
    mEffectMesh->SetBlendAdd(true);
    
    
    mParticle = std::make_unique<ParticleComponent>(this);
    mParticle->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/laser_part.png"));

    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_LASER);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/laser.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
}

void LaserActor::UpdateActor(float deltaTime)
{
    if (!isDisp) return;
    
    auto v = GetPosition();
    float speed = 1000.0f * deltaTime;
    SetPosition(Vector3(v.x, v.y, v.z+speed));
    if(v.z > 600)
    {
        Disappear();
    }
    
    
    collComp->SetDisp(isDisp);
    if (collComp->GetCollided())
    {
        for (auto col : collComp->GetTargetColliders())
        {
            if (col->GetColliderType() == C_ENEMY)
            {
                Disappear();
                break;
            }
        }
    }
}

void LaserActor::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    
    StageObjectActor::Appear(pos, type);
    mParticle->CreateParticles(Vector3(0,0,-100), 5, 5.f, 0.2f, 60.0f);
    mParticle->SetParticleSpeed(5);
    
    meshComp->SetVisible(true);
    mEffectMesh->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
}

void LaserActor::Disappear()
{
    isDisp = false;
    meshComp->SetVisible(false);
    mEffectMesh->SetVisible(false);
    mParticle->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetDisp(false);
}

