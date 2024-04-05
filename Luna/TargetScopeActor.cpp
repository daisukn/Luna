#include "TargetScopeActor.h"
#include "BillboardComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "Mesh.h"


TargetScopeActor::TargetScopeActor(Application* a, Stage* s)
    : StageObjectActor(a, s)
{
    isDisp = true;
    mCcopeBillboard = std::make_unique<BillboardComponent>(this, 200);
    mCcopeBillboard->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/target_scope.png"));
    mCcopeBillboard->SetVisible(true);
    SetPosition(Vector3(0, 0, 150));
    SetScale(0.1f);
}

void TargetScopeActor::UpdateActor(float deltaTime)
{

}
