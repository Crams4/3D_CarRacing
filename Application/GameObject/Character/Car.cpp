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

	m_Gravity += 0.05f;	//���L����
	m_mWorld._42 -= m_Gravity;	//�d�͒ǉ�

	// �L�����N�^�[�̈ړ����x(�^��������_���ł���)
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

	// �L�����N�^�[�̉�]�s���n��
	//UpdateRotate(moveVec);

	// �L�����N�^�[�̃��[���h�s���n�鏈��
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
	// �������͂������ꍇ�͏������Ȃ�
	if (srcMoveVec.LengthSquared() == 0.0f) { return; }

	// �ړ������̃x�N�g��
	Math::Vector3 targetDir = srcMoveVec;

	// �L�����̐��ʕ����̃x�N�g��
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// �Ԃ̊p�x
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
	//�n���X�^�[��n�ɗ���
	//=====================
	//�����蔻��(���C����)
	//=====================

	//���C����p�ϐ����쐬
	KdCollider::RayInfo rayInfo;
	//���C�̔��ˈʒu(���W)��ݒ�
	rayInfo.m_pos = GetPos();	//m_pos�͎����̍��W(����������(Pivot))
	//���C�̔��˕���
	rayInfo.m_dir = Math::Vector3::Down;
	//���ˈʒu�������
	//xrayInfo.m_pos.y += 0.1f;
	//�i���̋��e�͈�(�������l��)
	static float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;
	//���C�̒���
	rayInfo.m_range = m_Gravity + enableStepHigh;
	//�����蔻����������^�C�v��ݒ�
	rayInfo.m_type = KdCollider::TypeGround;

	//=====================
	//�f�o�b�N�p
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



	//���C�ɓ��������I�u�W�F�N�g���

	//���C�Ɠ����蔻�������!

	//��蕨����̏���
	m_wpRideObject.reset();
	//�I�u�W�F�N�g���X�g�ł̓����蔻��
	for (auto& wpGameObj : m_wpHitObjList)
	{
		std::shared_ptr<KdGameObject> spGameObj = wpGameObj.lock();
		if (spGameObj)
		{
			//���C�ɓ��������I�u�W�F�N�g���
			std::list<KdCollider::CollisionResult> retRayList;
			spGameObj->Intersects(rayInfo, &retRayList);

			//���C�ɓ����������X�g�����ԋ߂��I�u�W�F�N�g�����o
			//���C���Ւf���ꂽ��̒���
			float maxOverLap = 0;
			//�n�ʂ̍��W�i�[�p
			Math::Vector3 hitPos = {};
			//������t���O
			bool hit = false;
			for (auto& ret : retRayList)
			{
				//���C���Ւf���I�[�o�[������������Ԓ������̂�T��
				if (maxOverLap < ret.m_overlapDistance)
				{
					//�I�[�o�[���b�v�̍X�V
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hit = true;
				}
			}

			//�n�ʂɓ������Ă���
			if (hit)
			{
				//xMath::Vector3 m_pos = GetPos();
				//xm_pos = groundPos + Math::Vector3{ 0,-0.1f,0 };//���C�̔��˒n�_���𒲐�
				SetPos(hitPos);
				m_Gravity = 0;
				//��蕨�ɏ���Ă��鎞�̂�
				if (spGameObj->IsRideable())
				{
					Math::Matrix mInvertRideObject;
					//�t�s����擾
					spGameObj->GetMatrix().Invert(mInvertRideObject);

					//��蕨�Ɍ��_��ύX(��蕨�̈ړ��s��𓯊�)
					m_mLocalFromRideObject = m_mWorld * mInvertRideObject;

					//��蕨����
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