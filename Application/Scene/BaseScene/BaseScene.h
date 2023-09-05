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

	void AddObject(const std::shared_ptr<KdGameObject>& _obj)// &��t���邾���ŃR�s�[���������Ȃ�
	{
		m_spGameObjectList.push_back(_obj);
	}

protected:

	virtual void Event() {}
	virtual void Init() {}

	// �J����
	std::unique_ptr<KdCamera> m_camera = nullptr;
	//�I�u�W�F�N�g���Ԃ�����ꏊ(�A�h���X)���X�g
	std::list<std::shared_ptr<KdGameObject>> m_spGameObjectList;
};