#pragma once

class CameraBase;

enum class Status
{
	NEUTRAL,	//������
	DRIVE,		//�O�i
	RIGHT,		//�E����
	LEFT,		//������
	BACK		//���
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
	//�Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateCollisin();

	std::shared_ptr<KdModelWork>	m_spModel = nullptr;
	std::weak_ptr<CameraBase>			m_wpCamera;

	// ���[���h�s���n��ׂ̂��ꂼ��̉�]�p�x
	Math::Vector3						m_worldRot = Math::Vector3::Zero;

	//�Փ˂���I�u�W�F�N�g���X�g
	std::vector<std::weak_ptr<KdGameObject>> m_wpHitObjList;

	//�d��
	float m_Gravity = 0;
	//���x
	float moveSpd = 0;
	//�A�N�Z��(��)
	float power = 0;

	//�X�e�[�^�X�Ǘ��p
	Status state = Status::NEUTRAL;

	//���Ă������
	Math::Vector3 nowAng = Math::Vector3::Zero;

	//��蕨����֌W
	Math::Matrix	m_mLocalFromRideObject;
	std::weak_ptr<KdGameObject>	m_wpRideObject;

	///////���[�Y�ȃ��[�e�[�V����
	static const int ghostMax = 10;
	Math::Matrix m_ghostRotateMat[ghostMax];

	//�ʒu�ۑ�
	static const int saveNum = 180;
	Math::Matrix m_saveMat[saveNum];
	Math::Vector3 saveMove[saveNum];


};