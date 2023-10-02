#pragma once
#include "DirectXCommon.h"
#include "Audio.h"
//�O���錾
class SceneManager;

/// <summary>
/// �V�[�����
/// </summary>
class BaseScene
{
	
public:
	//���z�f�X�g���N�^
	virtual ~BaseScene() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon) = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { this->sceneManager = sceneManager; }
protected:
	//�V�[���}�l�[�W���[(�؂�Ă������)
	SceneManager* sceneManager = nullptr;
};

