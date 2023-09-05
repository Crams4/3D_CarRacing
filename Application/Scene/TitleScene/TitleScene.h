#pragma once
#include "../BaseScene/BaseScene.h"

class TitleScene:public BaseScene
{
public:
	TitleScene() { Init(); }
	~TitleScene() {
	//	m_tex.Release();
	//m_enter.Release();
	}

	void DrawSprite()override;

private:
	std::shared_ptr<KdTexture> m_tex;
	std::shared_ptr<KdTexture> m_enter;


	float alpha;

	bool  m_isReverse;
	float m_progress;


	Math::Matrix m_mat = Math::Matrix::Identity;

	void Event()override;
	void Init()override;

};
