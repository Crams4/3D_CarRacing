#include "BaseScene.h"

void BaseScene::PreUpdate()
{
	// 不要なオブジェクトを削除
	// オブジェクトリストの整理
	// auto …型推論
	// auto禁止
	// 範囲ベースFor イテレータ
	auto iteret = m_spGameObjectList.begin();
	while (iteret != m_spGameObjectList.end())
	{
		if ((*iteret)->IsExpired())
		{
			// 消す
			iteret = m_spGameObjectList.erase(iteret);
		}
		else
		{
			++iteret;// 次へ
		}
	}
	for (auto& obj : m_spGameObjectList)
	{
		obj->PreUpdate();// ポリモーフィズム
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
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->GenerateDepthMapFromLight();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawLit();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_spGameObjectList)
		{
			obj->DrawUnLit();//ポリモーフィズム
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
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
	// 2Dの描画はこの間で行う
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


