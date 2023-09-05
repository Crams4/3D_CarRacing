#include "GoalFlag.h"

void GoalFlag::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/goal.gltf");

		Math::Matrix trans, scale;
		trans = Math::Matrix::CreateTranslation({ -41,2.5f,-99 });
		scale = Math::Matrix::CreateScale(20,1,1);

		m_mWorld = scale * trans;

		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("Flag", m_spModel, KdCollider::TypeBump);

	}
}

void GoalFlag::Update()
{
}

void GoalFlag::DrawLit()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);

}

void GoalFlag::SetEvent()
{
}
