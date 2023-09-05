#pragma once

class CameraBase;

enum class Status
{
	NEUTRAL,	//無入力
	DRIVE,		//前進
	RIGHT,		//右旋回
	LEFT,		//左旋回
	BACK		//後退
};

class Car : public KdGameObject
{
public:
	Car() {}
	virtual ~Car()		override {}

	void Init()				override;
	void Update()			override;
	void DrawLit()			override;

	void SetCamera(const std::shared_ptr<CameraBase>& camera)
	{
		m_wpCamera = camera;
	}

	void RegsitHitObject(const std::shared_ptr<KdGameObject>& object)
	{
		m_wpHitObjList.push_back(object);
	}

	Math::Matrix GetGhostRotate() const override
	{
		return m_ghostRotateMat[ghostMax-1];
	}
	
	void CarRotation(float _angle = 1.5f);


private:

	void UpdateRotate(Math::Vector3& srcMoveVec);
	//衝突判定とそれに伴う座標の更新
	void UpdateCollisin();

	std::shared_ptr<KdModelWork>	m_spModel = nullptr;
	std::weak_ptr<CameraBase>			m_wpCamera;

	// ワールド行列を創る為のそれぞれの回転角度
	Math::Vector3						m_worldRot = Math::Vector3::Zero;

	//衝突するオブジェクトリスト
	std::vector<std::weak_ptr<KdGameObject>> m_wpHitObjList;

	//重力
	float m_Gravity = 0;
	//速度
	float moveSpd = 0;
	//アクセル(仮)
	float power = 0;

	//ステータス管理用
	Status state = Status::NEUTRAL;

	//見ている方向
	Math::Vector3 nowAng = Math::Vector3::Zero;

	//乗り物制御関係
	Math::Matrix	m_mLocalFromRideObject;
	std::weak_ptr<KdGameObject>	m_wpRideObject;

	///////ルーズなローテーション
	static const int ghostMax = 10;
	Math::Matrix m_ghostRotateMat[ghostMax];

	//位置保存
	static const int saveNum = 180;
	Math::Matrix m_saveMat[saveNum];
	Math::Vector3 saveMove[saveNum];


};