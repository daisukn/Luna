#include "UIElement.h"

#include "Application.h"
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"

UIElement::UIElement(Application* a)
    : mScore(0)
    , mPlayerLife(0)
    , mPlayerMaxLife(0)
    , mNumStages(0)
    , mTime(0)
{
    mGaugeActor = std::make_unique<Actor>(a);
    mGaugeActor->SetPosition(Vector3(-500, 300, 0));
    mGauge = std::make_unique<SpriteComponent>(mGaugeActor.get());
    mGauge->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge.png"));

    
    mGaugeFrameActor = std::make_unique<Actor>(a);
    mGaugeFrameActor->SetPosition(Vector3(-500, 300, 0));
    mGaugeFrame = std::make_unique<SpriteComponent>(mGaugeFrameActor.get(), 100, false);
    mGaugeFrame->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge_frame.png"));
    
}

UIElement::~UIElement()
{
}


void UIElement::Update()
{
    float sc = static_cast<float>(mPlayerLife) / static_cast<float>(mPlayerMaxLife);
    mGauge->SetScale(sc, 1.0f);
    float width = static_cast<float>(mGauge->GetTexWidth());
    mGaugeActor->SetPosition(Vector3(-500.f - (width -  width * sc) /2, 300, 0));
    
}
