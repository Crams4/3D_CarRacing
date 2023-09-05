#include "ResultScene.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../SceneManager.h"

void ResultScene::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mat);
	Math::Rectangle rc = { 0,0,1280,720 };
	Math::Color cr = { 1,1,1,alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex.get(), 0, 0, 1280, 720, &rc);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_enter.get(), 0, 0, &rc, &cr);
	m_mat = Math::Matrix::Identity;
	m_progress += 0.02f * (m_isReverse * -2 + 1);
	if (m_progress >= 1.0f)
	{
		m_isReverse = true;
	}
	else if (m_progress < -0.2f)
	{
		m_isReverse = false;
	}

	float progress = std::clamp(m_progress, 0.05f, 1.0f);
	float towardEndVec = 0.05f - 1.0f;
	alpha = 1 + towardEndVec * EaseInputsine(progress);
}



void ResultScene::Event()
{

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}

}

void ResultScene::Init()
{
	m_tex=KdAssets::Instance().m_textures.GetData("Asset/Textures/GOAL.png");
	m_enter =KdAssets::Instance().m_textures.GetData("Asset/Textures/PressEnter.png");
	
	//m_tex.Load("Asset/Textures/GOAL.png");
	//m_enter.Load("Asset/Textures/PressEnter.png");
	alpha = 1;
	m_isReverse = false;
	m_progress = 0;
}
