#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "Input.h"
#include "TouchableObject.h"
#include "Camera.h"
#include "FollowingCamera.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Framework.h"
#include "Material.h"
#include "PostEffect.h"
#include "Audio.h"
#include "Floor.h"
#include <sstream>
//前方宣言
class ParticleManager;

class Tutorial : public BaseScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// 障害物の発生データの読み込み
	/// </summary>
	void LoadFloorPopData();

	/// <summary>
	/// 障害物配置のコマンド更新
	/// </summary>
	void UpdataFloorPopCommand();


	void CheckAllCollision();

private:
	Audio* audio = nullptr;

	Input* input = nullptr;
	//障害物
	std::list<std::unique_ptr<Floor>> floors;
	std::stringstream floorPopCom;


	std::unique_ptr<PostEffect> postEffect;
	bool startFlag = false;
	float startEfRadius = 0;

	bool backFlag = false;
	float backEfRadius = 1000.f;
	//天球
	std::unique_ptr <Object3d> skyObj;
	std::unique_ptr<Framework> frame;
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;
	XMFLOAT3 cameraPos;
	float angle = 0.0f;
	//パーティクル
	ParticleManager* particleMan = nullptr;
	std::unique_ptr<Material> material;
	float alpha = 0.0f;
	//XMFLOAT3 cameraPos;
	std::unique_ptr<Player> player;
	int32_t rotateTime = 120;
	bool rotateFlag = false;
	XMFLOAT3 cameraTargetPos;
	int operatePhase = 0;
	XMFLOAT3 playerRot;
	XMFLOAT3 playerPos;
	XMFLOAT3 playerScale;
	float easFrame = 0.0f;
	// カメラ関係
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};

