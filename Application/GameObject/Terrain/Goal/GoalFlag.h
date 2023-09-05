#pragma once 

class GoalFlag:public KdGameObject
{
public:
	GoalFlag(){};
	~GoalFlag(){};

	void Init()				override;
	void Update()			override;
	void DrawLit()			override;

	bool IsGoal()const override { return true; }

	void SetEvent()			override;


private:

	std::shared_ptr<KdModelWork>	m_spModel = nullptr;


};
