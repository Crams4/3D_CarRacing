#include "BaseScene.h"

void BaseScene::PreUpdate()
{
	// �s�v�ȃI�u�W�F�N�g���폜
	// �I�u�W�F�N�g���X�g�̐���
	// auto �c�^���_
	// auto�֎~
	// �͈̓x�[�XFor �C�e���[�^
	auto iteret = m_spGameObjectList.begin();
	while (iteret != m_spGameObjectList.end())
	{
		if ((*iteret)->IsExpired())
		{
			// ����
			iteret = m_spGameObjectList.erase(iteret);
		}
		else
		{
			++iteret;// ����
		}
	}
	for (auto& obj : m_spGameObjectList)
	{
		obj->PreUpdate();// �|�����[�t�B�Y��
	}
}

void BaseScene::Update()
{
	for (auto& obj : m_spGameObjectList)
	{
		obj->Update();
	}
	Event();
}

void BaseScene::PostUpdate()
{
	for (auto& obj : m_spGameObjectList)
	{
		obj->PostUpdate();
	}
}

void BaseScene::ImGuiUpdate()
{
	for (auto& obj : m_spGameObjectList)
	{
		obj->ImGuiUpdate();
	}
}



void BaseScene::PreDraw()
{
	for (auto& obj : m_spGameObjectList)
	{
		obj->PreDraw();
	}
}

void BaseScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����Ղ�I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->GenerateDepthMapFromLight();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂���I�u�W�F�N�g(�s�����ȕ��̂�2D�L����)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawLit();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g(�����ȕ������܂ޕ��̂�G�t�F�N�g)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawUnLit();//�|�����[�t�B�Y��
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �����I�u�W�F�N�g(�������I�u�W�F�N�g��G�t�F�N�g)��Begin��End�̊Ԃɂ܂Ƃ߂�Draw����
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawBright();
		}
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void BaseScene::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2D�̕`��͂��̊Ԃōs��
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawSprite();
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::DrawDebug()
{
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawDebug();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
}


