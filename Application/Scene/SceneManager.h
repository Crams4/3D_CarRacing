#pragma once

class BaseScene;


class SceneManager
{
public:

	//SceneManager();
	//~SceneManager();

	//シーン情報
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

	//オブジェクトリスト関係
	//参照 (&) ※呼び出し先にコピーを作らない
	//const (定数)
	const std::list<std::shared_ptr<KdGameObject>>& GetObjectList();
	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

	void Release();


private:

	void ChangeScene(SceneType _sceneType);

	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	SceneType m_currentSceneType = SceneType::Result;
	SceneType m_nextSceneType = m_currentSceneType;

//!シングルトンパターン=============================
//便利すぎるので多用する人いるけど
//何でもかんでもシングルトンにすんな

	//生成するインスタンスの数を1つに制限するデザインパターン

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