#pragma once

#include "Component.h"

// スプライト管理 Componentを継承
class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* a, int order = 100, bool isBG = false);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* texture);

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

    void SetVisible(bool visible) { mIsVisible = visible; }
    bool GetVisible() const { return mIsVisible; }
    
    void SetScale(float w, float h) { mScaleWidth = w; mScaleHeight = h; }
    
protected:
    // スプライト用のテクスチャ
    class Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
    
    float mScaleWidth;
    float mScaleHeight;
    
    bool mIsBackGround;
    bool mIsVisible;


};


