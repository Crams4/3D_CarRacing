#pragma once
#include "../BaseScene/BaseScene.h"

class ResultScene :public BaseScene
{
public:
	ResultScene() { Init(); }
	~ResultScene() {
		//m_tex.Release();
		//m_enter.Release();
	}

	void DrawSprite() override;

private:

	void Event()override;
	void Init()override;


	std::shared_ptr<KdTexture> m_tex;
	std::shared_ptr<KdTexture> m_enter;
	//KdTexture m_tex;
	//KdTexture m_enter;

	float alpha;

	bool  m_isReverse;
	float m_progress;


	Math::Matrix m_mat = Math::Matrix::Identity;

};
