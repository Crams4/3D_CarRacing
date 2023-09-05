#include "SceneManager.h"
#include "BaseScene/BaseScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

#include "TitleScene/TitleScene.h"

void SceneManager::PreUpdate()
{
	//�V�[���؂�ւ�
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}

	//NULL�`�F�b�N
	if (!m_currentScene)return;
	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	if (!m_currentScene)return;
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	if (!m_currentScene)return;
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	if (!m_currentScene)return;
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	if (!m_currentScene)return;
	m_currentScene->Draw();
}

void SceneManager::PostDraw()
{
	if (!m_currentScene)return;
	/*m_currentScene->PostDraw();*/
}

void SceneManager::DrawSprite()
{
	if (!m_currentScene)return;
	m_currentScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	if (!m_currentScene)return;
	m_currentScene->DrawDebug();
}

void SceneManager::ImGuiUpdate()
{
	if (!m_currentScene)return;
	m_currentScene->ImGuiUpdate();
}


const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetObjectList()
{
	//���݂̃V�[���̃I�u�W�F�N�g���X�g���Q�b�g����
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_currentScene->AddObject(_obj);
}

void SceneManager::Release()
{
	//KdSafeRelease(m_currentScene);
	m_currentScene = nullptr;
}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	//���݂̃V�[�����폜
	Release();

	//���̃V�[�����쐬���A���݂̃V�[���ɐݒ肷��
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared< TitleScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared < GameScene>();
		break;
	case SceneType::Result:
		m_currentScene = std::make_shared < ResultScene>();
	default:
		break;
	}

	m_currentSceneType = _sceneType;

}
