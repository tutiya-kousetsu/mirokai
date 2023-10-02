#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
Player* Player::Create(Model* model)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}
	//���f���̃Z�b�g
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
	//�R���C�_�[�̒ǉ�
	float radius = 3.0f;
	//���a�����������畂�������W�����̒��S�ɂ���
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

//�����������Ȃ����p�̃A�b�v�f�[�g
void Player::StopUpdate()
{
	Object3d::Update();
}

//�ړ�
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

	// ���W�̕ύX�𔽉f
	Object3d::SetPosition(position);

}


void Player::Collision()
{
	Audio* audio = Audio::GetInstance();
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = Object3d::GetPosition();

	//��������
	if (!onGround) {
		//�������̉����x
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
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
			// �s��̍X�V�Ȃ�
			Object3d::Update();
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f)) {
			// ���n
			onGround = true;
		}
	}
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// �s��̍X�V�Ȃ�
	Object3d::Update();
}

//�X�P�[��������������p�֐�
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

//�X�P�[����傫������p�֐�
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

//�p�[�e�B�N���̐���
void Player::CreateParticle()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = Object3d::GetPosition();
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//�ǉ�
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