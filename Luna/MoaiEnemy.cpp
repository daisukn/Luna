#include "MoaiEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

const int MAX_MOAIBULLET = 50;
const int MAX_DONUTS = 8;

MoaiEnemy::MoaiEnemy(Application* a, Stage* s)
    : StageObjectActor(a, s)
    , mForwardSpeed(0.0f)
    , mAnglerSpeed(0.0f)
    , mUpperSpeed(0.0f)
    , mAngle(0.0f)
    , mLife(10)
    , mCntShot(0)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/moai.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);


    // 爆発
    mExplosion = std::make_unique<ExplosionActor>(a);
    mExplosion->SetScale(2);
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/moai.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 弾幕
    for (int i = 0; i < MAX_MOAIBULLET; i++)
    {
        mBullet.emplace_back(std::make_unique<BulletActor>(a, ownerStage));
    }
    
    for (int i = 0; i < MAX_DONUTS; i++)
    {
        mDonuts.emplace_back(std::make_unique<DonutsActor>(a, ownerStage));
    }
    
    // 移動用コンポーネント
    mMoveComp = std::make_unique<MoveComponent>(this);
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&MoaiEnemy::Behavior_0);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_1);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_2);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_3);

}

MoaiEnemy::~MoaiEnemy()
{
    mBullet.clear();
    mDonuts.clear();
}

void MoaiEnemy::UpdateActor(float deltaTime)
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


void MoaiEnemy::Behavior_0(float deltaTime)
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
            ShotDonuts();
        }
    }


    mMoveComp->SetForwardSpeed(mForwardSpeed);
    mMoveComp->SetAngularSpeed(mAnglerSpeed);
        
}


void MoaiEnemy::Behavior_1(float deltaTime)
{
    
}

void MoaiEnemy::Behavior_2(float deltaTime)
{
    
}

void MoaiEnemy::Behavior_3(float deltaTime)
{
    
}

void MoaiEnemy::CheckCllider()
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


void MoaiEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    state = StateNormal;
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    mLife = 10;
    mCntShot = 0;
}

void MoaiEnemy::Disappear()
{
    StageObjectActor::Disappear();
    isDisp = false;
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetCollided(false);
}

void MoaiEnemy::ShotDonuts()
{
    if (state != StateNormal)
    {
        return;
    }
    
    mCntShot++;
    float xOffset = 10.f;
    if (mCntShot % 2 == 0)
    {
        xOffset = -10.f;
    }
    
    Vector3 v = GetPosition();
    for (int i = 0; i < MAX_DONUTS; i++)
    {
        if (!mDonuts[i]->GetDisp())
        {
            mDonuts[i]->Appear(Vector3(v.x + xOffset, v.y, v.z-5 ), 0);
            break;
        }
    }
}
