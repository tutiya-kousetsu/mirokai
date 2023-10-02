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
#include "Audio.h"

class TitleScene : public BaseScene
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

private:
	Audio* audio = nullptr;
	std::unique_ptr <Sprite> titleSp;
	std::unique_ptr <Sprite> spaceSp;
	std::unique_ptr <Sprite> backSp;
	std::unique_ptr <Sprite> circleSp;
	DirectX::XMFLOAT2 size = {1000.0f, 1000.0f};
	Input* input = nullptr;
	//�V��
	std::unique_ptr <Object3d> skyObj;
	std::unique_ptr<DebugCamera> camera;
	XMFLOAT3 cameraPos;
	std::unique_ptr<Player> player;
	float angle = 0.0f;
};

