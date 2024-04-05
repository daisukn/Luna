#include "ShipEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "BoundingVolumeComponent.h"
#include "ColliderComponent.h"
#include "Mesh.h"
#include "Stage.h"

const int MAX_SHIPBULLET = 50;

ShipEnemy::ShipEnemy(Application* a, Stage* s)
    : StageObjectActor(a, s)
    , mLife(30)
    , mForwardSpeed(0.0f)
    , mAnglerSpeed(0.0f)
    , mAngle(0.0f)
{

    meshComp = std::make_unique<SkeletalMeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/ship2.fbx"));
    meshComp->SetAnimID(0, PLAY_CYCLIC);
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    SetScale(0.1f);
    
    // 爆発
    mExplosion = std::make_unique<ExplosionActor>(a);
    mExplosion->SetScale(4);


    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/ship.fbx")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 300, 500), Vector3(1, 0.5, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 弾幕
    for (int i = 0; i < MAX_SHIPBULLET; i++)
    {
        mBullet.emplace_back(std::make_unique<BulletActor>(a, ownerStage));
    }
    
    // 移動用コンポーネント
    moveComp = std::make_unique<MoveComponent>(this);
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&ShipEnemy::Behavior_0);
    BehaviorTable.push_back(&ShipEnemy::Behavior_1);
    BehaviorTable.push_back(&ShipEnemy::Behavior_2);
    BehaviorTable.push_back(&ShipEnemy::Behavior_3);
    
}

ShipEnemy::~ShipEnemy()
{
    
}



void ShipEnemy::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
    cntLifetime++;
    

    // 行動を反映
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
        
        
    if (state == StateNormal)
    {
        if (GetPosition().z < 0)
        {
            Disappear();
        }
        CheckCllider();
        
        
    }
    else if (state == StateExploted)
    {
        
        if (!mExplosion->GetDisp())
        {
            isDisp = false;
            Disappear();
        }
    }
}


void ShipEnemy::Behavior_0(float deltaTime)
{
    if (cntLifetime < 90)
    {
        mForwardSpeed = -300.f;
    }
    else if (cntLifetime < 600)
    {
        mForwardSpeed = 0.f;
    }
    else
    {
        mForwardSpeed = -100.f;
    }

    if (cntLifetime > 120 && cntLifetime < 600)
    {
        if (cntLifetime % 30 == 0)
        {
            //ShotDonuts();
        }
    }


    moveComp->SetForwardSpeed(mForwardSpeed);
    moveComp->SetAngularSpeed(mAnglerSpeed);
        
        
     
}

void ShipEnemy::Behavior_1(float deltaTime)
{
    
}

void ShipEnemy::Behavior_2(float deltaTime)
{
    
}

void ShipEnemy::Behavior_3(float deltaTime)
{
    
}


void ShipEnemy::CheckCllider()
{

    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_PLAYER
                   || col->GetColliderType() == C_LASER)
            {
                
                mLife--;
                if (mLife <= 0)
                {
                    meshComp->SetVisible(false);
                    collComp->GetBoundingVolume()->SetVisible(false);
                    collComp->SetCollided(false);
                    
                    state = StateExploted;
                    mExplosion->Appear(GetPosition());
                }
                break;
                    
            }
        }

    }
}

void ShipEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    state = StateNormal;
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    mLife = 30;
}

void ShipEnemy::Disappear()
{
    StageObjectActor::Disappear();
    isDisp = false;
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetCollided(false);
}
