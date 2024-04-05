#include "TitleStage.h"
#include "Application.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "SpriteComponent.h"

TitleStage::TitleStage(class Application* a)
    : Stage(a)
{
    mApp->GetRenderer()->SetClearColor(0.5, 0.5, 0.5);
    LoadStageData();
}

TitleStage::~TitleStage()
{
    UnloadStageData();
}

float ang  = 0.0f;

void TitleStage::LoadStageData()
{
    mMoonActor = std::make_unique<Actor>(mApp);
    mMoonActor->SetPosition(Vector3(10, 5, 30));
    mMoonActor->SetScale(0.05f);

    mMoonMesh = std::make_unique<MeshComponent>(mMoonActor.get());
    mMoonMesh->SetMesh(mApp->GetRenderer()->GetMesh("Assets/Models/moon.lwo"));
    

    mPlaneActor = std::make_unique<Actor>(mApp);
    mPlaneActor->SetScale(0.9f);
    Quaternion rot = Quaternion(Vector3::UnitY, Math::ToRadians(45));
    mPlaneActor->SetRotation(rot);
    mPlaneActor->SetPosition(Vector3(-5, -5, 30));

    mPlaneMesh = std::make_unique<SkeletalMeshComponent>(mPlaneActor.get());
    mPlaneMesh->SetMesh(mApp->GetRenderer()->GetMesh("Assets/Models/plane.fbx"));
    mPlaneMesh->SetAnimID(0, PLAY_CYCLIC);
    
    
    mSkyActor = std::make_unique<Actor>(mApp);
    mSkySprite = std::make_unique<SpriteComponent>(mSkyActor.get(), 100, true);
    mSkySprite->SetTexture(mApp->GetRenderer()->GetTexture("Assets/Textures/night_sky.jpg"));
    
    
    
    mIsQuitStage = false;
    
}

void TitleStage::UnloadStageData()
{
//    delete titleActor;
//    app->RemoveActor(titleActor);
    mApp->GetRenderer()->UnloadData();
    //std::cout << "Unload StageData()" << std::endl;
}

void TitleStage::StageInput(const struct InputState &state)
{
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_RETURN) == EReleased)
    {
        mIsQuitStage = true;
    }
}

void TitleStage::UpdateStage()
{
    ang -= 0.1f;
    if (ang > 360.0f) ang = 0.0f;
    Quaternion rot = Quaternion(Vector3::UnitY, Math::ToRadians(ang));
    mMoonActor->SetRotation(rot);

}
