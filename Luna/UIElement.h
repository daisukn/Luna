#pragma once

#include "SpriteComponent.h"
#include "Actor.h"
#include <memory>

class UIElement
{
public:
    UIElement(class Application* a);
    ~UIElement();
    
    void SetLife(int life) { mPlayerLife = life; }
    void SetMaxLife(int max) { mPlayerMaxLife = max; }
    
    void Update();

private:
    int mScore;
    int mPlayerLife;
    int mPlayerMaxLife;
    int mNumStages;
    int mTime;
    
    std::unique_ptr<class Actor> mGaugeActor;
    std::unique_ptr<class Actor> mGaugeFrameActor;
    std::unique_ptr<class SpriteComponent> mGauge;
    std::unique_ptr<class SpriteComponent> mGaugeFrame;
};
