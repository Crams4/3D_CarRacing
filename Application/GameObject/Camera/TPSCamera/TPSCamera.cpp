#include "TPSCamera.h"

void TPSCamera::Init()
{
	// ��_(�^�[�Q�b�g)�̖ڐ��̈ʒu
	m_LocalPos = Math::Matrix::CreateTranslation(0, 0.5f, -5.0f);
	m_mWorld = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPSCamera::Update()
{
	// �^�[�Q�b�g�̍s��(�L���ȏꍇ���p����)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{

			//m_ghostRotateMat[0] = Math::Matrix::CreateFromYawPitchRoll(spTarget->GetMatrix().ToEuler());
			//for (int i = ghostMax - 1; i >= 0; i--)
			//{
			//	m_ghostRotateMat[i] = m_ghostRotateMat[i - 1];
			//}
			//Math::Matrix targetRotate =  m_ghostRotateMat[ghostMax - 1];
			Math::Matrix targetRotate = Math::Matrix::CreateFromYawPitchRoll(spTarget->GetGhostRotate().ToEuler());
			targetMat = targetRotate * Math::Matrix::CreateTranslation(spTarget->GetPos());
			/*targetMat = spTarget->GetMatrix();*/
				
		}
	}

	// �J�����̉�]
	//UpdateRotateByMouse();
	m_Rotation = GetRotationMatrix();
	m_mWorld = m_LocalPos * m_Rotation * targetMat;

	CameraBase::Update();
}

void TPSCamera::UpdateRotateByMouse()
{
	// �}�E�X�ʒu�̍����𓾂�
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]������ׂɊe���̉�]�p�x��ݒ肷��
	m_DegAng.x += mouseMove.y * 0.15f;
	m_DegAng.y += mouseMove.x * 0.15f;

	// ��]����
	m_DegAng.x = std::clamp(m_DegAng.x, -FLT_MAX, FLT_MAX);
}
