#include "Car.h"
#include "../Camera/CameraBase.h"
#include "../../Scene/SceneManager.h"
#include "../../main.h"

void Car::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Car/car.gltf");
		m_Gravity = 0.5f;
		//m_mWorld = Math::Matrix::CreateRotationY(180);
		m_mWorld = Math::Matrix::CreateScale(0.5f);
		SetPos({ -91,4.0f,-100 });
		m_pCollider = std::make_unique<KdCollider>();
		//m_debugWire = std::make_unique<KdDebugWireFrame>();
	}
}

void Car::Update()
{

	m_Gravity += 0.05f;	//万有引力
	m_mWorld._42 -= m_Gravity;	//重力追加

	// キャラクターの移動速度(真似しちゃダメですよ)
	Math::Vector3 nowPos = GetPos();

	Math::Vector3 moveVec = Math::Vector3::Zero;

	moveVec.z = 1.0f;
	moveSpd += power * 0.5f;
	if (moveSpd > 0) moveSpd -= 0.01f;

	if (GetAsyncKeyState('W'))
	{
		power += 0.001f;
		state = Status::DRIVE;
	}
	else
	{
		power = 0;
	}
	if (GetAsyncKeyState('S'))
	{
		//moveVec.z = -1.0f;
		//moveSpd -= 0.1f;
		state = Status::BACK;
	}
	if (GetAsyncKeyState('A'))
	{
		CarRotation();
		//KdAudioManager::Instance().PauseAllSound();

		//KdAudioManager::Instance().Play("Asset/Audio/Curve.wav");
		state = Status::LEFT;
	}
	if (GetAsyncKeyState('D'))
	{
		//moveVec.x = 0.5f;
		//KdAudioManager::Instance().PauseAllSound();
		//KdAudioManager::Instance().Play("Asset/Audio/Curve.wav");
		CarRotation();
		state = Status::RIGHT;
	}


	if (GetAsyncKeyState('L'))
	{
		nowPos = { 0,3.0f,-100 };
	}
	if (GetAsyncKeyState('P'))
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Result);
	}


	moveSpd = std::clamp(moveSpd, 0.0f, 1.5f);

	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		/*moveVec = moveVec.TransformNormal
		(moveVec, spCamera->GetRotationYMatrix());*/
		moveVec = moveVec.TransformNormal
		(moveVec, Math::Matrix::CreateRotationY(m_worldRot.y));
	}

	moveVec.Normalize();
	moveVec *= moveSpd;

	nowPos.x += moveVec.x;
	nowPos.y += moveVec.y;
	nowPos.z += moveVec.z;

	// キャラクターの回転行列を創る
	//UpdateRotate(moveVec);

	// キャラクターのワールド行列を創る処理
	//Math::Matrix rotation =
	//Math::Matrix::CreateRotationY(
	//DirectX::XMConvertToRadians(m_worldRot.y));
	//m_mWorld = rotation * Math::Matrix::CreateTranslation(nowPos);





	SetPos(nowPos);

	m_saveMat[0] = Math::Matrix::CreateFromYawPitchRoll(m_worldRot) * Math::Matrix::CreateScale(0.5f) * Math::Matrix::CreateTranslation(nowPos);;
	saveMove[0] = moveVec;
	for (int i = saveNum - 1; i > 0; i--)
	{
		m_saveMat[i] = m_saveMat[i - 1];
		saveMove[i] = saveMove[i - 1];
	}



	if (nowPos.y < 1)
	{
		//m_isExpired = true;
		moveVec = saveMove[saveNum - 1];
		m_mWorld = m_saveMat[saveNum - 1];
		CarRotation();
		/*moveVec = moveVec.TransformNormal
		(moveVec, Math::Matrix::CreateRotationY(saveWorldRot.y[save]));*/
	}

	m_ghostRotateMat[0] = Math::Matrix::CreateFromYawPitchRoll(GetMatrix().ToEuler());
	for (int i = ghostMax - 1; i > 0; i--)
	{
		m_ghostRotateMat[i] = m_ghostRotateMat[i - 1];
	}
	UpdateCollisin();
}

void Car::DrawLit()
{
	if (!m_spModel) return;

	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}



void Car::UpdateRotate(Math::Vector3& srcMoveVec)
{
	// 何も入力が無い場合は処理しない
	if (srcMoveVec.LengthSquared() == 0.0f) { return; }

	// 移動方向のベクトル
	Math::Vector3 targetDir = srcMoveVec;

	// キャラの正面方向のベクトル
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// 間の角度
	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -8.0f, 8.0f);
	m_worldRot.y += rotateAng;
}

void Car::UpdateCollisin()
{
	//ハムスター大地に立つ
	//=====================
	//当たり判定(レイ判定)
	//=====================

	//レイ判定用変数を作成
	KdCollider::RayInfo rayInfo;
	//レイの発射位置(座標)を設定
	rayInfo.m_pos = GetPos();	//m_posは自分の座標(しかも足元(Pivot))
	//レイの発射方向
	rayInfo.m_dir = Math::Vector3::Down;
	//発射位置を微調整
	//xrayInfo.m_pos.y += 0.1f;
	//段差の許容範囲(加速分考慮)
	static float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;
	//レイの長さ
	rayInfo.m_range = m_Gravity + enableStepHigh;
	//当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	//=====================
	//デバック用
	//=====================
	/*if(m_debugWire)
	{
		m_debugWire->AddDebugLine
		(
			rayInfo.m_pos,
			rayInfo.m_dir,
			rayInfo.m_range
		);
	}*/



	//レイに当たったオブジェクト情報

	//レイと当たり判定をする!

	//乗り物判定の消去
	m_wpRideObject.reset();
	//オブジェクトリストでの当たり判定
	for (auto& wpGameObj : m_wpHitObjList)
	{
		std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
		if (spGameObj)
		{
			//レイに当たったオブジェクト情報
			std::list<KdCollider::CollisionResult> retRayList;
			spGameObj->Intersects(rayInfo, &retRayList);

			//レイに当たったリストから一番近いオブジェクトを検出
			//レイが遮断された先の長さ
			float maxOverLap = 0;
			//地面の座標格納用
			Math::Vector3 hitPos = {};
			//当たりフラグ
			bool hit = false;
			for (auto& ret : retRayList)
			{
				//レイを遮断しオーバーした長さが一番長いものを探す
				if (maxOverLap < ret.m_overlapDistance)
				{
					//オーバーラップの更新
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hit = true;
				}
			}

			//地面に当たっている
			if (hit)
			{
				//xMath::Vector3 m_pos = GetPos();
				//xm_pos = groundPos + Math::Vector3{ 0,-0.1f,0 };//レイの発射地点分を調整
				SetPos(hitPos);
				m_Gravity = 0;
				//乗り物に乗っている時のみ
				if (spGameObj->IsRideable())
				{
					Math::Matrix mInvertRideObject;
					//逆行列を取得
					spGameObj->GetMatrix().Invert(mInvertRideObject);

					//乗り物に原点を変更(乗り物の移動行列を同期)
					m_mLocalFromRideObject = m_mWorld * mInvertRideObject;

					//乗り物判定
					m_wpRideObject = spGameObj;
				}

			}
		}
	}


}

void Car::CarRotation(float _ang)
{

	Math::Vector3 pos = m_mWorld.Translation();
	Math::Vector3 rotate = m_mWorld.ToEuler();
	if (state == Status::RIGHT)
	{
		rotate.y += DirectX::XMConvertToRadians(_ang);
		m_worldRot.y = rotate.y;
	}
	else if (state == Status::LEFT)
	{
		rotate.y -= DirectX::XMConvertToRadians(_ang);
		m_worldRot.y = rotate.y;
	}
	m_mWorld = Math::Matrix::CreateFromYawPitchRoll(rotate) * Math::Matrix::CreateScale(0.5f) * Math::Matrix::CreateTranslation(pos);


}