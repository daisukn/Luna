#include "ExplosionActor.h"
#include "Application.h"
#include "Renderer.h"


ExplosionActor::ExplosionActor(Application* a)
    : Actor(a)
    , mIsDisp(false)
    , mCntLifetime(0)
{
    
    explosionPart = std::make_unique<ParticleComponent>(this);
    explosionPart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/explosion.png"));

    
    smokePart = std::make_unique<ParticleComponent>(this, 90);
    //smokePart->SetBlendAdd(true);
    smokePart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/smoke.png"));

    
    flashPart = std::make_unique<ParticleComponent>(this, 110);
    flashPart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/flash.png"));
    //flashPart->SetBlendAdd(true);
    flashPart->SetParticleSpeed(4.0f);
//    flashPart->SetBlendAdd(true);

}

void ExplosionActor::UpdateActor(float deltaTime)
{
    if(!mIsDisp) { return; }
    
    mCntLifetime++;
    
    if (mCntLifetime == 1)
    {
        flashPart->CreateParticles(Vector3(0,0,0), 5, 0.4f, 0.1f, 25.0f);
    }
    if (mCntLifetime == 5)
    {
        explosionPart->CreateParticles(Vector3(0,0,0), 10, 0.6f, 0.5f, 15.0f);
    }
    if (mCntLifetime == 20)
    {
        smokePart->CreateParticles(Vector3(0,0.1,0), 5, 0.6f, 0.4f, 20.0f);
    }
    if (mCntLifetime == 60)
    {
        mIsDisp = false;
    }
    
    auto v = GetPosition();
    SetPosition(Vector3(v.x, v.y - 0.5f, v.z));
 }

void ExplosionActor::Appear(Vector3 pos)
{
    mCntLifetime = 0;
    mIsDisp = true;
    SetPosition(pos);
 }
