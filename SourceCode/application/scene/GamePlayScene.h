#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "Tutorial.h"
#include "TitleScene.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "Floor.h"
#include "Player.h"
#include "TouchableObject.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Shake.h"
#include "Audio.h"
#include <memory>
#include <list>
#include <sstream>

//前方宣言
class ParticleManager;
/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
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
	
	//イーズイン用frame
	float inFrame = 0.f;
	float easFrame = 0.f;
	
	std::unique_ptr<Sprite> spriteBG;
	// カメラ
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	std::unique_ptr<PostEffect> postEffect;
	
	//ゲームが終わった時に暗くするやつの半径
	float endEfRadius = 1000;
	bool endFlag = false;
	//障害物
	std::list<std::unique_ptr<Floor>> floors;
	std::stringstream floorPopCom;
	//プレイヤー
	std::unique_ptr<Player> player;
	//天球
	std::unique_ptr<Object3d> skyObj;
	std::unique_ptr<Object3d> object;

	//パーティクル
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	BaseCollider* collider = nullptr;
	//ポジション
	//ドア
	XMFLOAT3 doorPos;
	//自機
	XMFLOAT3 playerPos;
	//自機のスケール
	XMFLOAT3 playerScale;
	//ローテーション
	XMFLOAT3 doorRot;
	XMFLOAT3 playerRot;

	//フラグ
	//フェーズ
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