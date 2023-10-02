#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}
	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

Player::~Player()
{
}

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	particleMan = ParticleManager::GetInstance();

	Object3d::SetRotation({ 0,0,0 });
	Object3d::SetScale({ 1.25f,1.25f,1.25f });
	camera.reset(new FollowingCamera());
	shake.reset(new Shake());
	//コライダーの追加
	float radius = 3.0f;
	//半径だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	AudioLoad();
	return true;
}

void Player::AudioLoad()
{


}

void Player::Update()
{
	Audio* audio = Audio::GetInstance();
	move();
	Collision();
	Input* input = Input::GetInstance();
	Object3d::Update();
	Object3d::SetPosition(position);
}

//動かしたくない時用のアップデート
void Player::StopUpdate()
{
	Object3d::Update();
}

//移動
void Player::move()
{
	Input* input = Input::GetInstance();
	rotation = Object3d::GetRotation();
	position = Object3d::GetPosition();

	float rotate = 3.0f;
	float moveSpeed = 0.1f;

	if (input->TriggerKey(DIK_W) && moveTime == 0) { moveF2 = true; spaceF = true; }
	if (moveF2) { position.z += moveSpeed = 0.1f; rotation.x += rotate; moveTime++; }
	if (moveTime == 30) { moveF2 = false; moveTime = 0; spaceF = false; pushT = 0; }

	if (input->TriggerKey(DIK_S) && moveTime == 0) { moveF3 = true; spaceF = true; }
	if (moveF3) { position.z -= moveSpeed = 0.1f; rotation.x -= rotate; moveTime++; }
	if (moveTime == 30) { moveF3 = false; moveTime = 0; spaceF = false; pushT = 0; }

	if (input->TriggerKey(DIK_D) && moveTime == 0) { moveF = true; spaceF = true; }
	if (moveF) { position.x += moveSpeed = 0.1f; rotation.z -= rotate; moveTime++; }
	if (moveTime == 30) { moveF = false; moveTime = 0; spaceF = false; pushT = 0; }

	if (input->TriggerKey(DIK_A) && moveTime == 0) { moveF1 = true; spaceF = true; }
	if (moveF1) { position.x -= moveSpeed = 0.1f; rotation.z += rotate; moveTime++; }
	if (moveTime == 30) { moveF1 = false; moveTime = 0; spaceF = false; pushT = 0; }

	// 座標の変更を反映
	Object3d::SetPosition(position);

}


void Player::Collision()
{
	Audio* audio = Audio::GetInstance();
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = Object3d::GetPosition();

	//落下処理
	if (!onGround) {
		//下向きの加速度
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//移動
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}

	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		const float adsDistance = 0.18f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.4f);
			// 行列の更新など
			Object3d::Update();
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f)) {
			// 着地
			onGround = true;
		}
	}
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 行列の更新など
	Object3d::Update();
}

//スケールを小さくする用関数
void Player::ScaleSmall()
{
	scale = Object3d::GetScale();
	if (scale.x >= 0.f && scale.y >= 0.f && scale.z >= 0.f) {
		scale.x -= 0.01f;
		scale.y -= 0.01f;
		scale.z -= 0.01f;
		Object3d::SetScale(scale);
	}
}

//スケールを大きくする用関数
void Player::ScaleLarge()
{
	scale = Object3d::GetScale();
	if (scale.x <= 0.9f && scale.y <= 0.9f && scale.z <= 0.9f) {
		scale.x += 0.01f;
		scale.y += 0.01f;
		scale.z += 0.01f;
		Object3d::SetScale(scale);
	}

}

void Player::OnCollision(int i)
{
}

//パーティクルの生成
void Player::CreateParticle()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = Object3d::GetPosition();
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);

	}
}

void Player::Draw()
{
	Object3d::Draw();
}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos{};

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}