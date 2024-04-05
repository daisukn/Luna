#include "Stage.h"
#include "Application.h"


#include <fstream>
#include <sstream>
#include <algorithm>

Stage::Stage(class Application* a)
    : mApp(a)
    , mIsQuitStage(false)
    , mNumLayout(0)
    , mCntLayout(0)
    , mPlayerPos(Vector3(0,0,0))
{
    
}

Stage::~Stage()
{
    UnloadStageData();
}

void Stage::LoadStageData()
{
    // overrideする
}

void Stage::LoadStageLayout(std::string filename)
{
    // 一旦クリア
    mLayout.clear();
    mNumLayout = 0;
    mCntLayout = 0;
    
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        // 空白行と、#で始まる行は読み飛ばす
        if (line.empty())
        {
            continue;
        }
        if (line[0] == '#' || line[0] == ',')
        //if (!std::isdigit(line[0]))
        {
            continue;
        }
        std::istringstream ss(line);
        std::string frame;
        std::string x;
        std::string y;
        std::string z;
        std::string objType;
        std::string behaveType;
        
        // カンマで分割
        std::getline(ss, frame, ',');
        std::getline(ss, objType, ',');
        std::getline(ss, behaveType, ',');
        std::getline(ss, x, ',');
        std::getline(ss, y, ',');
        std::getline(ss, z, ',');
        
        // データ変換
        StageLayout sl;
        sl.frame = std::stoi(frame);
        sl.objType = std::stoi(objType);
        sl.behaveType = std::stoi(behaveType);
        sl.x = std::stof(x);
        sl.y = std::stof(y);
        sl.z = std::stof(z);
        
        mLayout.emplace_back(sl);
        
    }
    
    // フレームでソート
    std::sort(mLayout.begin(), mLayout.end(), [](const StageLayout& a, const StageLayout& b){
         return a.frame < b.frame;
     });
    
    mNumLayout = static_cast<int>(mLayout.size());
    file.close();
    
}

void Stage::AddStageActor(StageObjectActor* a)
{
    mStageActors.push_back(a);
}

void Stage::AppearLayout()
{
    // overrideする
}

void Stage::UnloadStageData()
{
    // overrideする
}

void Stage::UpdateStage()
{
    // overrideする
}

void Stage::ProcessInput(const struct InputState &state)
{
    StageInput(state);
}

void Stage::StageInput(const struct InputState &state)
{
    
}


