#include "BulletActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "BillboardComponent.h"

BulletActor::BulletActor(Application* a, class Stage* s)
    : StageObjectActor(a, s)
    , angle(0.0f)
    , xSpeed(0.0f)
    , ySpeed(0.0f)
    , zSpeed(0.0f)
{
    
    
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/bullet.lwo"));
    meshComp->SetVisible(false);
    SetScale(0.15f);

/*    flare = std::make_unique<BillboardComponent>(this);
    flare->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/flare.png"));
    flare->SetScale(0.1f);
    flare->SetVisible(false);
*/
    smoke = std::make_unique<BillboardComponent>(this);
    smoke->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/smoke_add.png"));
    smoke->SetScale(0.2);
    smoke->SetBlendAdd(true);
    smoke->SetVisible(false);

    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_BULLET);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/bullet.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&BulletActor::Behavior_0);
    BehaviorTable.push_back(&BulletActor::Behavior_1);
    BehaviorTable.push_back(&BulletActor::Behavior_2);
    BehaviorTable.push_back(&BulletActor::Behavior_3);
}

void BulletActor::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
        
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
    
    

    auto v = GetPosition();
    SetPosition(Vector3(v.x + xSpeed, v.y + ySpeed, v.z + zSpeed));

    if(v.z < 0)
    {
        Disappear();
    }
    
    CheckCllider();
}



void BulletActor::Behavior_0(float deltaTime)
{
    float speed = 600.0f * deltaTime;

    zSpeed = -speed;
}

void BulletActor::Behavior_1(float deltaTime)
{
    float speed = 1000.0f * deltaTime;
    
    xSpeed = cos(Math::ToRadians(angle)) * speed * deltaTime;
    ySpeed = sin(Math::ToRadians(angle)) * speed * deltaTime;
    zSpeed = -6.f;

}

void BulletActor::Behavior_2(float deltaTime)
{
}
void BulletActor::Behavior_3(float deltaTime)
{
}

void BulletActor::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    //flare->SetVisible(true);
    smoke->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
}

void BulletActor::Disappear()
{
    isDisp = false;
    meshComp->SetVisible(false);
    //flare->SetVisible(false);
    smoke->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetDisp(false);
}

void BulletActor::CheckCllider()
{
    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_PLAYER)
            {
                Disappear();

                break;
                
            }
        }

    }
    
}
