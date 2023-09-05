#pragma once
class BaseScene
{
public:
	BaseScene(){}
	~BaseScene(){}

	 void PreUpdate();
	 void Update();
	 void PostUpdate();

	 void ImGuiUpdate();

	 void PreDraw();
	 void Draw();

	 virtual void DrawSprite();
	 void DrawDebug();


	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()
	{
		return m_spGameObjectList;
	}

	void AddObject(const std::shared_ptr<KdGameObject>& _obj)// &を付けるだけでコピーが発生しない
	{
		m_spGameObjectList.push_back(_obj);
	}

protected:

	virtual void Event() {}
	virtual void Init() {}

	// カメラ
	std::unique_ptr<KdCamera> m_camera = nullptr;
	//オブジェクト実態がある場所(アドレス)リスト
	std::list<std::shared_ptr<KdGameObject>> m_spGameObjectList;
};