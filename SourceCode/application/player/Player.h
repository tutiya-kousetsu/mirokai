#pragma once
#include "Object3d.h"
#include "GameObject.h"
#include "Shake.h"
#include "Input.h"
#include "FollowingCamera.h"
#include "Framework.h"
#include "Audio.h"
#include <memory>
#include <list>
#include <forward_list>
#include <DirectXMath.h>
class ParticleManager;

using namespace DirectX;

class Player : public Object3d
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);
public:
	//コンストラクタ
	//Player();

	~Player();

	bool Initialize() override;

	void AudioLoad();

	//更新
	void Update() override;

	//動かしたくない時用更新
	void StopUpdate();

	//移動
	void move();

	void Collision();

	//スケールを小さくする
	void ScaleSmall();

	void ScaleLarge();

	//描画
	void Draw();

	//当たり判定
	void OnCollision(int i);

	//パーティク生成
	void CreateParticle();
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	ParticleManager* particleMan = nullptr;
	bool alive = true;
	bool onGround = true;
	bool moveF = false;
	bool moveF1 = false;
	bool moveF2 = false;
	bool moveF3 = false;
	bool spaceF = false;
	//bool move
	int32_t moveTime = 0;
	int32_t pushT = 0;
	//落下ベクトル
	XMVECTOR fallV;
	//シェイク用
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
	//音
	Audio* audio = nullptr;
	std::unique_ptr<FollowingCamera> camera;
};

