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
//�O���錾
class ParticleManager;

class Tutorial : public BaseScene
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

	Input* input = nullptr;
	//��Q��
	std::list<std::unique_ptr<Floor>> floors;
	std::stringstream floorPopCom;


	std::unique_ptr<PostEffect> postEffect;
	bool startFlag = false;
	float startEfRadius = 0;

	bool backFlag = false;
	float backEfRadius = 1000.f;
	//�V��
	std::unique_ptr <Object3d> skyObj;
	std::unique_ptr<Framework> frame;
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;
	XMFLOAT3 cameraPos;
	float angle = 0.0f;
	//�p�[�e�B�N��
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

