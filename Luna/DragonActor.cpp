#include "DragonActor.h"
#include "MeshComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "MoveComponent.h"



DragonActor::DragonActor(Application* app)
    : Actor(app)
    , mAnimID(0)
    //, mPrevPos(Vector3::Zero)
{
    // メッシュ初期化
    mMeshComp = std::make_unique<SkeletalMeshComponent>(this);
    mMeshComp->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/dragon.fbx"));
    mMeshComp->SetAnimID(0, PLAY_CYCLIC);
    
    mMeshComp->SetToonRender(true, 1.003f);
    mMeshComp->SetVisible(true);
    
    
    // 場所調整
    SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    SetScale(0.10f);
    
}
    



void DragonActor::ApplyMotion()
{

    mMeshComp->SetAnimID(mAnimID, PLAY_CYCLIC);

}



void DragonActor::UpdateActor(float deltaTime)
{

 
}



void DragonActor::SetVisible(bool visible)
{
    mMeshComp->SetVisible(visible);
}


