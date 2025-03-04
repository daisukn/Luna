#pragma once

#include "Math.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>


// ディレクショナルライト
struct DirectionalLight{
    Vector3 Direction;      // 向き
    Vector3 DiffuseColor;   // 色
    Vector3 SpecColor;      // 反射色
};


// 描画エンジン
class Renderer
{
public:
    // コンストラクタ
    Renderer();
    // デストラクタ
    virtual ~Renderer();
      
    // 初期化
    bool Initialize(std::string title, float scWidth, float scHeight);
    // 描画（Applicationから呼ばれる）
    void Draw();
    // 終了
    void Shutdown();

    void SetClearColor(float r, float g, float b);


    // ビューマトリックスを設定する
    void SetViewMatrix(const Matrix4& view) { mInvView = mViewMatrix = view; mInvView.Invert();}
    Matrix4 GetViewMatrix() const { return mViewMatrix; }
    Matrix4 GetInvViewMatrix() const { return mInvView; }

    // スクリーンサイズのGtter
    float GetScreenWidth() const { return mScreenWidth; }
    float GetScreenHeight() const { return mScreenHeight; }
      
    
    // スプライトコンポーネント登録・削除
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    // 背景スプライトコンポーネント登録・削除
    void AddBackGroundSprite(class SpriteComponent* sprite);
    void RemoveBackGroundSprite(class SpriteComponent* sprite);
    // メッシュコンポーネント登録・削除
    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(class MeshComponent* mesh);
    // 背景メッシュコンポーネント登録・削除
    void AddBackGroudMeshComp(class MeshComponent* mesh);
    void RemoveBackGroudMeshComp(class MeshComponent* mesh);
    // エフェクトメッシュコンポーネント登録・削除
    void AddEffectMeshComp(class MeshComponent* mesh);
    void RemoveEffectMeshComp(class MeshComponent* mesh);
    // パーティクルコンポーネント登録・削除
    void AddParticleComp(class ParticleComponent* part);
    void RemoveParticleComp(class ParticleComponent* part);
    // ビルボード登録・削除
    void AddBillboardComp(class BillboardComponent* billboard);
    void RemoveBillboardComp(class BillboardComponent* billboard);
    // デバッガーコンポーネント登録・削除
    void AddDebuggerComp(class DebuggerComponent* dbg);
    void RemoveDebuggerComp(class DebuggerComponent* dbg);
    
    
    // メッシュ登録
    class Mesh* GetMesh(const std::string& fileName);
    // テクスチャ登録
    class Texture* GetTexture(const std::string& fileName);


    // データ解放
    void UnloadData();
    
    // アンビエントライト設定
    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }
    
    
private:
    // ウィンドウタイトル
    std::string mStrTitle;
    // Windowハンドラ
    SDL_Window* mWindow;
    // GLコンテキスト
    SDL_GLContext mGLContext;

    // ビューマトリックス
    Matrix4 mViewMatrix;
    Matrix4 mInvView;
    // プロジェクションマトリックス
    Matrix4 mPojectionMatrix;
    //スクリーンサイズ
    float mScreenWidth;
    float mScreenHeight;
    
    

    // メッシュ用シェーダー
    std::unique_ptr<class Shader> mMeshShader;
    std::unique_ptr<class Shader> mMeshShaderToon;
    std::unique_ptr<class Shader> mBackGroundShader;
    // スキンメッシュ用シェーダー
    std::unique_ptr<class Shader> mSkinnedShader;
    std::unique_ptr<class Shader> mSkinnedShaderToon;
    // スプライト用シェーダー
    std::unique_ptr<class Shader> mSpriteShader;
    // ビルボード用シェーダー
    std::unique_ptr<class Shader> mBillboardShader;
    // パーティクル用シェーダー
    std::unique_ptr<class Shader> mParticleShader;
    // ワイヤーフレーム用　単色シェーダー
    std::unique_ptr<class Shader> mSolidShader;
    // シェーダー一括ロード
    bool LoadShaders();

    // スプライト用ポリゴン（Billboardでも使う）
    std::unique_ptr<class VertexArray> mSpriteVerts;
    // スプライト用ポリゴンの生成
    void CreateSpriteVerts();
    
    // Lightsingをシェーダーに送る
    void SetLightUniforms(class Shader* shader);
    
    // アセット
    std::unordered_map<std::string, std::unique_ptr<class Texture>> mTextures;
    std::unordered_map<std::string, std::unique_ptr<class Mesh>> mMeshes;

    // コンポーネント
    std::vector<class SpriteComponent*> mSpriteComps;
    std::vector<class SpriteComponent*> mBgSpriteComps;
    std::vector<class MeshComponent*> mMeshComps;
    std::vector<class MeshComponent*> mBgMesh;
    std::vector<class MeshComponent*> mEffectMesh;
    std::vector<class SkeletalMeshComponent*> mSkeletalMeshes;
    std::vector<class ParticleComponent*> mParticleComps;
    std::vector<class BillboardComponent*> mBillboardComps;
    std::vector<class DebuggerComponent*> mDbgComps;
    
    
    // ライト
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;
    



};

