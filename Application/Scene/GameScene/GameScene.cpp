#include "GameScene.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Character/Car.h"
#include "../../GameObject/Terrain/Terrain.h"
#include "../../GameObject/Terrain/Goal/GoalFlag.h"
#include "../../Scene/SceneManager.h"



void GameScene::Event()
{
	//Math::Matrix transMat;

	////カメラの座標行列
	//transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 2.0f, -5.0f));
	//m_camera->SetCameraMatrix(transMat);
}

void GameScene::Init()
{
	//KdAudioManager::Instance().StopAllSound();
	//KdAudioManager::Instance().Play("Asset/Audio/StopCar.wav",true);

	//===================================================================
	// ステージ初期化
	//===================================================================
	std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>();
	terrain->Init();
	m_spGameObjectList.push_back(terrain);
//===================================================================
	// はむはむ初期化
	//===================================================================
	std::shared_ptr<Car> car = std::make_shared<Car>();
	car->Init();
	car->RegsitHitObject(terrain);
	m_spGameObjectList.push_back(car);

	std::shared_ptr<GoalFlag> goal = std::make_shared<GoalFlag>();
	goal->Init();
	car->RegsitHitObject(goal);
	m_spGameObjectList.push_back(goal);

	//===================================================================
	// カメラ初期化
	//===================================================================
	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	camera->Init();
	camera->SetTarget(car);
	car->SetCamera(camera);
	m_spGameObjectList.push_back(camera);


}


void GameScene::Release()
{
}
