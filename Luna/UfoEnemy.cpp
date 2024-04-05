#include "UfoEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "ColliderComponent.h"
#include "ParticleComponent.h"
#include "MoveComponent.h"

const int MAX_BULLET = 50;

UfoEnemy::UfoEnemy(Application* a, class Stage* s)
    : StageObjectActor(a, s)
    , mForwardSpeed(0.0f)
    , mAnglerSpeed(0.0f)
    , mUpperSpeed(0.0f)
    , mAngle(0.0f)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/ufo.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    
    // 爆発
    mExplosion = std::make_unique<ExplosionActor>(a);
    
    // 弾幕
    for(int i = 0; i < MAX_BULLET; i++)
    {
        mBullets.emplace_back(std::make_unique<BulletActor>(a, ownerStage));
    }
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/ufo.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 移動用コンポーネント
    mMoveComp = std::make_unique<MoveComponent>(this);
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&UfoEnemy::Behavior_0);
    BehaviorTable.push_back(&UfoEnemy::Behavior_1);
    BehaviorTable.push_back(&UfoEnemy::Behavior_2);
    BehaviorTable.push_back(&UfoEnemy::Behavior_3);
}

UfoEnemy::~UfoEnemy()
{
    mBullets.clear();
    BehaviorTable.clear();
}

void UfoEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    mAnglerSpeed = 0.0f;
    mForwardSpeed = 0.0f;
    mAngle = 0.0f;
    mMoveComp->Reset();
    
    meshComp->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    collComp->SetDisp(isDisp);

}

void UfoEnemy::Disappear()
{
    StageObjectActor::Disappear();
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
}


void UfoEnemy::CheckCllider()
{

    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_PLAYER
                   || col->GetColliderType() == C_LASER)
            {
                meshComp->SetVisible(false);
                collComp->GetBoundingVolume()->SetVisible(false);
                collComp->SetCollided(false);
                    
                state = StateExploted;
                mExplosion->Appear(GetPosition());
                break;
                    
            }
        }

    }
}

void UfoEnemy::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
    cntLifetime++;
    

    // 行動を反映
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
        
        
    if(state == StateNormal)
    {
    
        
        if(cntLifetime > 1000)
        {
            Disappear();
        }
        CheckCllider();
        
        
    }
    else if(state == StateExploted)
    {
        
        if(!mExplosion->GetDisp())
        {
            isDisp = false;
        }
    }
}



void UfoEnemy::Behavior_0(float deltaTime)
{


    float speed = 150.f;
    
    if (cntLifetime < 100)
    {
        mForwardSpeed = speed;
//        std::cout << GetPosition().z << std::endl;

    }
    if (cntLifetime == 220)
    {
        mForwardSpeed = speed;
        if (GetPosition().x > 0)
        {
            mAnglerSpeed = -75;
        }
        else
        {
            mAnglerSpeed = 75;
        }
    }
    if (cntLifetime == 440)
    {
        mAnglerSpeed = 0.0f;
    }
    
    if (cntLifetime > 220 && cntLifetime < 440)
    {
        if (cntLifetime % 10 == 0)
        {
            ShotLiner();
        }
    }
    mMoveComp->SetForwardSpeed(mForwardSpeed);
    mMoveComp->SetAngularSpeed(mAnglerSpeed);


}

void UfoEnemy::Behavior_1(float deltaTime)
{

    float speed = 100.f;
    mAngle += 5.f;
    
    if (cntLifetime < 100)
    {
        mForwardSpeed = -speed;
//        std::cout << GetPosition().z << std::endl;
        
        if (cntLifetime % 5 == 0)
        {
            ShotCircle();
        }
    }
    if (cntLifetime > 240)
    {
        mForwardSpeed = 0;
        mUpperSpeed = 100 * sin(Math::ToRadians(mAngle));
    }

    
    if (cntLifetime > 240)
    {
        if (cntLifetime % 5 == 0)
        {
            ShotLiner();
        }
    }
    if (cntLifetime > 500)
    {
        mForwardSpeed = -speed;
    }
    mMoveComp->SetForwardSpeed(mForwardSpeed);
    mMoveComp->SetUpSpeed(mUpperSpeed);


}

void UfoEnemy::Behavior_2(float deltaTime)
{

}

void UfoEnemy::Behavior_3(float deltaTime)
{
    
}


void UfoEnemy::ShotLiner()
{
    if (state != StateNormal)
    {
        return;
    }
    for (int i = 0; i < MAX_BULLET; i++)
    {
        if(!mBullets[i]->GetDisp())
        {
            auto v = GetPosition();
            mBullets[i]->Appear(Vector3(v.x, v.y, v.z), 0);
            break;
        }
    }

}


void UfoEnemy::ShotCircle()
{
    if (state != StateNormal)
    {
        return;
    }

    
    for(int j = 0;  j < 6; j++)
    {
        for(int i = 0; i < MAX_BULLET; i++)
        {
            if(!mBullets[i]->GetDisp())
            {
                mBullets[i]->SetAngle((float)j*60);
                mBullets[i]->Appear(GetPosition(), 1);
                break;
            }
        }
    }
    for(int i = 0; i < MAX_BULLET; i++)
    {
        if(!mBullets[i]->GetDisp())
        {
            mBullets[i]->Appear(GetPosition(), 0);
            break;
        }
    }
}

