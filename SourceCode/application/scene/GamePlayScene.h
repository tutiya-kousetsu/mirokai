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

//�O���錾
class ParticleManager;
/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GamePlayScene : public BaseScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update() override;


	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;
	
	/// <summary>
	/// ��Q���̔����f�[�^�̓ǂݍ���
	/// </summary>
	void LoadFloorPopData();

	/// <summary>
	/// ��Q���z�u�̃R�}���h�X�V
	/// </summary>
	void UpdataFloorPopCommand();

	void CheckAllCollision();
private:
	Audio* audio = nullptr;
	
	//�C�[�Y�C���pframe
	float inFrame = 0.f;
	float easFrame = 0.f;
	
	std::unique_ptr<Sprite> spriteBG;
	// �J����
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	std::unique_ptr<PostEffect> postEffect;
	
	//�Q�[�����I��������ɈÂ������̔��a
	float endEfRadius = 1000;
	bool endFlag = false;
	//��Q��
	std::list<std::unique_ptr<Floor>> floors;
	std::stringstream floorPopCom;
	//�v���C���[
	std::unique_ptr<Player> player;
	//�V��
	std::unique_ptr<Object3d> skyObj;
	std::unique_ptr<Object3d> object;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	BaseCollider* collider = nullptr;
	//�|�W�V����
	//�h�A
	XMFLOAT3 doorPos;
	//���@
	XMFLOAT3 playerPos;
	//���@�̃X�P�[��
	XMFLOAT3 playerScale;
	//���[�e�[�V����
	XMFLOAT3 doorRot;
	XMFLOAT3 playerRot;

	//�t���O
	//�t�F�[�Y
	// �J�����֌W
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};