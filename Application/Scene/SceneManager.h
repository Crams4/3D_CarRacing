#pragma once

class BaseScene;


class SceneManager
{
public:

	//SceneManager();
	//~SceneManager();

	//�V�[�����
	enum class SceneType
	{
		Title,
		Game,
		Result
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void PostDraw();

	void DrawSprite();
	void DrawDebug();

	void ImGuiUpdate();

	void Init()
	{
		ChangeScene(SceneType::Result);
	}

	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}

	//�I�u�W�F�N�g���X�g�֌W
	//�Q�� (&) ���Ăяo����ɃR�s�[�����Ȃ�
	//const (�萔)
	const std::list<std::shared_ptr<KdGameObject>>& GetObjectList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	void Release();


private:

	void ChangeScene(SceneType _sceneType);

	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	SceneType m_currentSceneType = SceneType::Result;
	SceneType m_nextSceneType = m_currentSceneType;

//!�V���O���g���p�^�[��=============================
//�֗�������̂ő��p����l���邯��
//���ł�����ł��V���O���g���ɂ����

	//��������C���X�^���X�̐���1�ɐ�������f�U�C���p�^�[��

private:
	SceneManager() {  };
	~SceneManager() {};
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
//==================================================
};