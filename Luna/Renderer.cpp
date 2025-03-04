#include "Renderer.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "ParticleComponent.h"
#include "BillboardComponent.h"

#include "DebuggerComponent.h"

#include <GL/glew.h>
#include <algorithm>

#define __GAME_DEBUG
//#define __FULLSCREEN__

// コンストラクタ
Renderer::Renderer()
    : mStrTitle("")
    , mScreenWidth(0.0f)
    , mScreenHeight(0.0f)
    , mWindow(nullptr)
    , mGLContext(nullptr)
{
}

// デストラクタ
Renderer::~Renderer()
{
}


// ウィンドウ生成とGL初期化
bool Renderer::Initialize(std::string title, float scWidth, float scHeight)
{
    mStrTitle        = title;    // ウィンドウタイトル
    mScreenWidth     = scWidth;  // スクリーン幅
    mScreenHeight    = scHeight; // スクリーン高さ
    
    
    // OpenGL プロファイル, バージョン
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // ダブルバッファリング
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // RGBAチャンネル
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // ハードウェアアクセラレーション有効化
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    

#ifdef __FULLSCREEN__
    unsigned int WINDOW_FLAGS = SDL_WINDOW_OPENGL || SDL_WINDOW_FULLSCREEN;
#else
    unsigned int WINDOW_FLAGS = SDL_WINDOW_OPENGL;
#endif // __FULLSCREEN__

    
    //ウインドウ生成
    mWindow = SDL_CreateWindow(mStrTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), WINDOW_FLAGS);
    
    if (!mWindow )
    {
        std::cout << "Unable to create window" << std::endl;
        return false;
    }
    
    
    // OpenGL コンテキスト生成
    mGLContext = SDL_GL_CreateContext(mWindow);
    
    // GLEW初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }
   
    
    /*シェーダー のロードなどはここでやる*/
    LoadShaders();
    CreateSpriteVerts();
 
    //
    SetClearColor(0.596f, 0.733f, 0.858f);
    SetClearColor(0.5f, 0.0f, 0.0f);

    return true;
}

// リリース処理
void Renderer::Shutdown()
{
    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);

}

void::Renderer::SetClearColor(float r,float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

// 描画処理
void Renderer::Draw()
{
//    glClearColor(0.596f, 0.733f, 0.858f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Culling設定
    glEnable( GL_CULL_FACE );
    glFrontFace(GL_CCW);

    
    /* 描画処理 メッシュ、スプライト*/
    // アルファブレンディングを有効化
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    
    // 背景スプライト
    glDisable(GL_DEPTH_TEST);
    mSpriteShader->SetActive();
    for (auto sprite : mBgSpriteComps)
    {
        sprite->Draw(mSpriteShader.get());
    }
    glEnable(GL_DEPTH_TEST);
 
    
    
    // 背景用メッシュ描画
    mBackGroundShader->SetActive();
    mBackGroundShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(mBackGroundShader.get());
    for (auto bg : mBgMesh)
    {
        if (bg->GetVisible())
        {
            bg->Draw(mBackGroundShader.get());
        }
    }
    
    
    
    // メッシュ描画
    mMeshShader->SetActive();
    for (auto mc : mMeshComps)
    {
        if (mc->GetVisible())
        {
            if(mc->GetToon())
            {
                mMeshShaderToon->SetActive();
                SetLightUniforms(mMeshShaderToon.get());
                mMeshShaderToon->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
                mc->Draw(mMeshShaderToon.get());
            }
            else
            {
                mMeshShader->SetActive();
                SetLightUniforms(mMeshShader.get());
                mMeshShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
                mc->Draw(mMeshShader.get());
            }
        }
    }

    // スキンメッシュ描画
    mSkinnedShaderToon->SetActive();
    mSkinnedShaderToon->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(mSkinnedShaderToon.get());
    for (auto sk : mSkeletalMeshes)
    {
        if (sk->GetVisible())
        {
            sk->Draw(mSkinnedShaderToon.get());
        }
    }

#ifdef __GAME_DEBUG
    // デバッガー用の描画
    mSolidShader->SetActive();
    mSolidShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
    mSolidShader->SetVectorUniform("uSolColor", Vector3(0.2f, 0.2f, 1.0f));
    // Update lighting uniforms
    SetLightUniforms(mSolidShader.get());
    for (auto dg : mDbgComps)
    {
        dg->Draw(mSolidShader.get());

    }
#endif // __GAME_DEBUG
    
    
    
    // パーティクルの処理
    // Zバッファに書き込まない
    glDepthMask(GL_FALSE);
    //加算合成
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_ONE, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteVerts->SetActive();
    mParticleShader->SetActive();
    mParticleShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);

    for (auto parts : mParticleComps)
    {
        parts->Draw(mParticleShader.get());
    }
    glDepthMask(GL_TRUE);
    
    
    
    
    // ビルボード
    // Zバッファに書き込まない
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteVerts->SetActive();
    mBillboardShader->SetActive();
    SetLightUniforms(mBillboardShader.get());
    mBillboardShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);

    for (auto bb : mBillboardComps)
    {
        
        bb->Draw(mBillboardShader.get());
    }
    glDepthMask(GL_TRUE);
    
    
    
    
    // スプライト処理
    glDisable(GL_DEPTH_TEST);
    // アルファブレンディング
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteShader->SetActive();
    for (auto sprite : mSpriteComps)
    {
        sprite->Draw(mSpriteShader.get());
    }
    glEnable(GL_DEPTH_TEST);
    
    
    

    // エフェクトメッシュ描画
    mMeshShader->SetActive();
    mMeshShader->SetMatrixUniform("uViewProj", mViewMatrix * mPojectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(mMeshShader.get());
    for (auto mesh : mEffectMesh)
    {
        if (mesh->GetVisible())
        {
            mesh->Draw(mMeshShader.get());
        }
    }
    
    


    SDL_GL_SwapWindow(mWindow);

}

// シェーダー 読み込み
bool Renderer::LoadShaders()
{

    // スプライト用シェーダー生成
    mSpriteShader = std::make_unique<Shader>();
    if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
    {
        return false;
    }
    mSpriteShader->SetActive();
    
    // ビューマトリックス、プロジェクションマトリックス
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    
    
    // Billboard用シェーダー
    mBillboardShader = std::make_unique<Shader>();
    if (!mBillboardShader->Load("Shaders/Billboard.vert", "Shaders/Billboard.frag"))
    {
        return false;
    }
    mBillboardShader->SetActive();
    
    
    // Particle用シェーダー
    mParticleShader = std::make_unique<Shader>();
    if (!mParticleShader->Load("Shaders/Billboard.vert", "Shaders/Particle.frag"))
    {
        return false;
    }
    mParticleShader->SetActive();
    
    // メッシュ用シェーダー生成
    mMeshShader = std::make_unique<Shader>();
    if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    mMeshShader->SetActive();
    
    // メッシュ用シェーダー(Toon)生成
    mMeshShaderToon = std::make_unique<Shader>();
    if (!mMeshShaderToon->Load("Shaders/Phong.vert", "Shaders/Toon.frag"))
    {
        return false;
    }
    mMeshShaderToon->SetActive();
    
    
    
    // 背景用シェーダー生成
    mBackGroundShader = std::make_unique<Shader>();
    if (!mBackGroundShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
    {
        return false;
    }
    mBackGroundShader->SetActive();
    
    
    
    // スキンメッシュ用シェーダー
    mSkinnedShader = std::make_unique<Shader>();
    if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    mSkinnedShader->SetActive();
    
    // スキンメッシュ用シェーダー(Toon)
    mSkinnedShaderToon = std::make_unique<Shader>();
    if (!mSkinnedShaderToon->Load("Shaders/Skinned.vert", "Shaders/Toon.frag"))
    {
        return false;
    }
    mSkinnedShaderToon->SetActive();
    
    
    // ワイヤフレーム用　単色シェーダー生成
    mSolidShader = std::make_unique<Shader>();
    if (!mSolidShader->Load("Shaders/BasicMesh.vert", "Shaders/SolidColor.frag"))
    {
        return false;
    }
    mSolidShader->SetActive();
    

    
    // ビューマトリックス、プロジェクションマトリックス（デフォルト値）
    mViewMatrix = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    mPojectionMatrix = Matrix4::CreatePerspectiveFOV(Math::ToRadians(30.0f), mScreenWidth, mScreenHeight, 1.0f, 2000.0f);
    
    // シェーダーに送る
    //meshShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    //skinnedShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    //billboardShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    
    return true;

}

// ライト設定
void Renderer::SetLightUniforms(Shader* shader)
{
    // カメラポジション
    Matrix4 invView = mViewMatrix;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    // Directional light
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.Direction);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.DiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.SpecColor);
    
    
    // フォグ
    shader->SetFloatUniform("uFoginfo.maxDist", 800.0f);
    shader->SetFloatUniform("uFoginfo.minDist", 400.0f);
    
//    shader->SetVectorUniform("uFoginfo.color", Vector3(0.75f, 0.96f, 0.99f) );
//    shader->SetVectorUniform("uFoginfo.color", Vector3(0.69f, 0.859f, 0.894f) );
    shader->SetVectorUniform("uFoginfo.color", Vector3(0.69f, 0.859f, 0.894f) );


}

// スプライトコンポーネントの登録
void Renderer::AddSprite(SpriteComponent* sprite)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = sprite->GetDrawOrder();
    auto iter = mSpriteComps.begin();
    for (;iter != mSpriteComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    mSpriteComps.insert(iter, sprite);
}

// スプライト削除
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSpriteComps.begin(), mSpriteComps.end(), sprite);
    if (iter != mSpriteComps.end())
    { // 要素が見つかった場合のみ削除
        mSpriteComps.erase(iter);
    }

}

// 背景スプライトコンポーネントの登録
void Renderer::AddBackGroundSprite(SpriteComponent* sprite)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = sprite->GetDrawOrder();
    auto iter = mBgSpriteComps.begin();
    for (;iter != mBgSpriteComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    mBgSpriteComps.insert(iter, sprite);
}

// スプライト削除
void Renderer::RemoveBackGroundSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mBgSpriteComps.begin(), mBgSpriteComps.end(), sprite);
    if (iter != mBgSpriteComps.end())
    { // 要素が見つかった場合のみ削除
        mBgSpriteComps.erase(iter);
    }

}



// テクスチャ取り出し
Texture* Renderer::GetTexture(const std::string &fileName){
 
    Texture* tex = nullptr;
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second.get();
    }
    else
    {
        std::unique_ptr<Texture> t = std::make_unique<Texture>();
        if(t->Load(fileName))
        {
            tex = t.get();
            mTextures.emplace(fileName, std::move(t));
        }
        else
        {
            tex = nullptr;
        }
    }
    return tex;
}

//スプライト用ポリゴン
void Renderer::CreateSpriteVerts()
{
    const float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
        0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
        -0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
    };
    

    const unsigned int indices[] = {
        2, 1, 0,
        0, 3, 2
    };
    mSpriteVerts = std::make_unique<VertexArray>((float*)vertices, (unsigned int)4, (unsigned int*)indices, (unsigned int)6);
}



// メッシュ取り出し
Mesh* Renderer::GetMesh(const std::string& fileName)
{
    Mesh* m = nullptr;
    auto iter = mMeshes.find(fileName);
    if (iter != mMeshes.end())
    {
        m = iter->second.get();
    }
    else
    {
        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
        
        if (mesh->Load(fileName, this))
        {
            m = mesh.get();
            mMeshes.emplace(fileName, std::move(mesh));
        }
        else
        {
            m = nullptr;
        }
    }
    return m;
}

// メッシュコンポーネント登録
void Renderer::AddMeshComp(MeshComponent* mesh)
{
    if (mesh->GetIsSkeletal())
    {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
        mSkeletalMeshes.emplace_back(sk);
    }
    else
    {

        mMeshComps.emplace_back(mesh);
    }
 }


// メッシュコンポーネント削除
void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
    if (mesh->GetIsSkeletal())
    {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
        auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
        if (iter != mSkeletalMeshes.end())
        { // 要素が見つかった場合のみ削除
            mSkeletalMeshes.erase(iter);
        }
    }
    else
    {
        auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
        if (iter != mMeshComps.end())
        { // 要素が見つかった場合のみ削除
            mMeshComps.erase(iter);
        }
    }
}

// BGメッシュコンポーネント登録
void Renderer::AddBackGroudMeshComp(MeshComponent* mesh)
{
    mBgMesh.emplace_back(mesh);
}


// BGメッシュコンポーネント削除
void Renderer::RemoveBackGroudMeshComp(MeshComponent* mesh)
{
    auto iter = std::find(mBgMesh.begin(), mBgMesh.end(), mesh);
    if (iter != mBgMesh.end())
    { // 要素が見つかった場合のみ削除
        mBgMesh.erase(iter);
    }
 
}

// エフェクトメッシュコンポーネント登録
void Renderer::AddEffectMeshComp(MeshComponent* mesh)
{
    mEffectMesh.emplace_back(mesh);
}


// BGメッシュコンポーネント削除
void Renderer::RemoveEffectMeshComp(MeshComponent* mesh)
{
    auto iter = std::find(mEffectMesh.begin(), mEffectMesh.end(), mesh);
    if (iter != mEffectMesh.end())
    { // 要素が見つかった場合のみ削除
        mEffectMesh.erase(iter);
    }
 
}


// パーティクルコンポーネント登録
void Renderer::AddParticleComp(ParticleComponent* part)
{
    int myDrawOrder = part->GetDrawOrder();
    auto iter = mParticleComps.begin();
    for (;iter != mParticleComps.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }
    mParticleComps.insert(iter, part);
}

// パーティクルコンポーネント削除
void Renderer::RemoveParticleComp(ParticleComponent* part)
{
    auto iter = std::find(mParticleComps.begin(), mParticleComps.end(), part);
    if (iter != mParticleComps.end())
    { // 要素が見つかった場合のみ削除
        mParticleComps.erase(iter);
    }
}


// ビルボードコンポーネント登録
void Renderer::AddBillboardComp(BillboardComponent* billboard)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = billboard->GetDrawOrder();
    auto iter = mBillboardComps.begin();
    for (;iter != mBillboardComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    mBillboardComps.insert(iter, billboard);
    
    
}

// パーティクルコンポーネント登録
void Renderer::RemoveBillboardComp(BillboardComponent* billboard)
{
    auto iter = std::find(mBillboardComps.begin(), mBillboardComps.end(), billboard);
    if (iter != mBillboardComps.end())
    { // 要素が見つかった場合のみ削除
        mBillboardComps.erase(iter);
    }
}




// デバッガーコンポーネント登録
void Renderer::AddDebuggerComp(DebuggerComponent* dbg)
{
    mDbgComps.emplace_back(dbg);
}

// パーティクルコンポーネント登録
void Renderer::RemoveDebuggerComp(DebuggerComponent* dbg)
{
    auto iter = std::find(mDbgComps.begin(), mDbgComps.end(), dbg);
    if (iter != mDbgComps.end())
    { // 要素が見つかった場合のみ削除
        mDbgComps.erase(iter);
    }
}



// データ解放
void Renderer::UnloadData()
{
    // テクスチャ削除
    mTextures.clear();
    
    // メッシュ削除
    mMeshes.clear();
    
}
