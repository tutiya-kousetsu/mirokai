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
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(Model* model = nullptr);
public:
	//�R���X�g���N�^
	//Player();

	~Player();

	bool Initialize() override;

	void AudioLoad();

	//�X�V
	void Update() override;

	//�����������Ȃ����p�X�V
	void StopUpdate();

	//�ړ�
	void move();

	void Collision();

	//�X�P�[��������������
	void ScaleSmall();

	void ScaleLarge();

	//�`��
	void Draw();

	//�����蔻��
	void OnCollision(int i);

	//�p�[�e�B�N����
	void CreateParticle();
	//���[���h���W���擾
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
	//�����x�N�g��
	XMVECTOR fallV;
	//�V�F�C�N�p
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
	//��
	Audio* audio = nullptr;
	std::unique_ptr<FollowingCamera> camera;
};

