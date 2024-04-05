#include "CloudStage.h"
#include "Application.h"
#include "MeshComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "PlaneActor.h"
#include "DragonActor.h"
#include "CloudBGActor.h"
#include "UfoEnemy.h"
#include "ShipEnemy.h"
#include "LaserActor.h"
#include "UIElement.h"

#include <cstdlib>
#include <ctime>

const int MAX_CLOUD = 50;
const int MAX_UFO = 50;
const int MAX_MOAI = 10;
const int MAX_SHIP = 5;

const float APEAR_POINT = 800.f;
const std::string stageFileName = "Setting/stage.csv";
//const std::string stageFileName = "Setting/Stage1.txt";


#define RANDOM_DEBUG
//#define STAGE_DEBUG

CloudStage::CloudStage(class Application* a)
    : Stage(a)
    , mStageCounter(0)
{
    LoadStageData();
    std::srand((unsigned int)std::time(nullptr));
    
}

CloudStage::~CloudStage()
{
    UnloadStageData();
}


void CloudStage::LoadStageData()
{
    mDragonActor = std::make_unique<DragonActor>(mApp);
    mDragonActor->SetPosition(Vector3(0, 0, 100));
    mDragonActor->SetVisible(false);

    
    for (int i = 0; i < MAX_CLOUD; i++)
    {
        mCloudActor.emplace_back( std::make_unique<CloudBGActor>(mApp) );
    }
    for (int i = 0;  i < MAX_UFO; i++)
    {
        mUfoEnemy.emplace_back( std::make_unique<UfoEnemy>(mApp, this));
    }
    for (int i = 0;  i < MAX_MOAI; i++)
    {
        mMoaiEnemy.emplace_back( std::make_unique<MoaiEnemy>(mApp, this));
    }
    for (int i = 0;  i < MAX_SHIP; i++)
    {
        mShipEnemy.emplace_back( std::make_unique<ShipEnemy>(mApp, this));
    }
    
    

    mSkyActor = std::make_unique<Actor>(mApp);
    mSkyActor->SetPosition(Vector3(0, 0, 500));
    Quaternion q(Vector3::UnitY, Math::ToRadians(180));
    mSkyActor->SetRotation(q);
    mSkyActor->SetScale(1);
    mSkyMesh = std::make_unique<MeshComponent>(mSkyActor.get(), false, MESH_BG);
    mSkyMesh->SetMesh(mApp->GetRenderer()->GetMesh("Assets/Models/sky.lwo"));
    
    // 飛行機
    mPlaneActor = std::make_unique<PlaneActor>(mApp, this);
    mPlaneActor->SetPosition(Vector3(0, 0, 30));
    
    LoadStageLayout(stageFileName);

    // UI
    mUiElement = std::make_unique<UIElement>(mApp);
    mUiElement->SetMaxLife(mPlaneActor->GetMaxLife());

    mIsQuitStage = false;
    mApp->GetRenderer()->SetClearColor(0.596f, 0.733f, 0.858f);
}

void CloudStage::UnloadStageData()
{
}

void CloudStage::StageInput(const struct InputState &state)
{
    
}


void CloudStage::UpdateStage()
{
    
    mPlayerPos = mPlaneActor->GetPosition();
    mPlayerLife = mPlaneActor->GetLife();
    mUiElement->SetLife(mPlayerLife);
    
    for (auto a : mStageActors)
    {
        a->SetPlayerPosition(mPlayerPos);
    }
    
    
    
    mStageCounter++;
    GenerateCloud();
    mUiElement->Update();

#ifdef RANDOM_DEBUG
    AppearRandom();
#else
    AppearLayout();
#endif // RANDOM_DEBUG

#ifdef STAGE_DEBUG
    DebugAppear();
#endif //STAGE_DEBUG
}

void CloudStage::GenerateCloud()
{
    if(mStageCounter % 2 == 0)
    {
        for(int i = 0; i < MAX_CLOUD; i++)
        {
            if(!mCloudActor[i]->GetDisp())
            {
                mCloudActor[i]->SetDisp(true);
                mCloudActor[i]->SetPosition(Vector3(std::rand() % 600 - 300, std::rand() % 400 - 200 , APEAR_POINT));
                break;
            }
        }
    }
}


void CloudStage::AppearLayout()
{

    if(mNumLayout <= mCntLayout)
    {
        return;
    }
    
    
    while(mLayout[mCntLayout].frame == mStageCounter)
    {
        switch(mLayout[mCntLayout].objType)
        {
            case 1: // UFO
                for(int i = 0; i < MAX_UFO; i++)
                {
                    if(!mUfoEnemy[i]->GetDisp())
                    {
                        mUfoEnemy[i]->Appear(Vector3(mLayout[mCntLayout].x, mLayout[mCntLayout].y ,mLayout[mCntLayout].z), mLayout[mCntLayout].behaveType);
                        break;
                    }
                }
                break;
            case 2: // moai
                for(int i = 0; i < MAX_MOAI; i++)
                {
                    if(!mMoaiEnemy[i]->GetDisp())
                    {
                        mMoaiEnemy[i]->Appear(Vector3(mLayout[mCntLayout].x, mLayout[mCntLayout].y ,mLayout[mCntLayout].z), mLayout[mCntLayout].behaveType);
                        break;
                    }
                }
                break;
            case 3: // ship
                for(int i = 0; i < MAX_SHIP; i++)
                {
                    if(!mShipEnemy[i]->GetDisp())
                    {
                        mShipEnemy[i]->Appear(Vector3(mLayout[mCntLayout].x, mLayout[mCntLayout].y ,mLayout[mCntLayout].z), mLayout[mCntLayout].behaveType);
                        break;
                    }
                }
                break;
        }
        mCntLayout++;
        if (mCntLayout >= mLayout.size())
        {
            break;
        }
    }

}


void CloudStage::AppearRandom()
{
    if(mStageCounter % 40 == 0)
    {
        for(int i = 0; i < MAX_UFO; i++)
        {
            if(!mUfoEnemy[i]->GetDisp())
            {
                mUfoEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , 0), 0);
                break;
            }
        }
    }
    
    if(mStageCounter % 60 == 0)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!mMoaiEnemy[i]->GetDisp())
            {
                mMoaiEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
    
    if(mStageCounter % 60 == 0)
    {
        for(int i = 0; i < MAX_SHIP; i++)
        {
            if(!mShipEnemy[i]->GetDisp())
            {
                mShipEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
}

void CloudStage::DebugAppear()
{
    if(mStageCounter % 500 == 0)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!mMoaiEnemy[i]->GetDisp())
            {
                mMoaiEnemy[i]->Appear(Vector3(-50, 0 , APEAR_POINT), 0);
                break;
            }
        }
    }
    if(mStageCounter % 500 == 250)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!mMoaiEnemy[i]->GetDisp())
            {
                mMoaiEnemy[i]->Appear(Vector3(50, 0 , APEAR_POINT), 0);
                break;
            }
        }
    }
}
