#include "CloudBGActor.h"
#include "Application.h"
#include "Renderer.h"


CloudBGActor::CloudBGActor(Application* a)
    : Actor(a)
    , mIsDisp(false)
{
    mCloudBillboard = std::make_unique<BillboardComponent>(this);
    mCloudBillboard->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/cloud_1.png"));
    mCloudBillboard->SetScale(1.3f);
}

void CloudBGActor::UpdateActor(float deltaTime)
{
    if (mIsDisp)
    {
        float speed = 250.0f * deltaTime;
        mCloudBillboard->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z-speed));
        if(v.z < 0)
        {
            mIsDisp = false;
            mCloudBillboard->SetVisible(false);
        }
    }
}
