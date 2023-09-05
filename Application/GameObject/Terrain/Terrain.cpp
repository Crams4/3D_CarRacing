#include "Terrain.h"

void Terrain::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Stage.gltf");
		//m_spModel->SetModelData("Asset/Data/Sample/Terrain/Stage.gltf");
		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("StageModel", m_spModel, KdCollider::TypeGround);
		m_mWorld = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
		m_mWorld = Math::Matrix::CreateScale(5);
	}
}

void Terrain::Update()
{
}

void Terrain::DrawLit()
{
	if (!m_spModel) return;

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel,m_mWorld);
}

