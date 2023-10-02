#pragma once
#include "Fbx_Model.h"
#include "Camera.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
class Fbx_Object3d
{
protected://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public://�T�u�N���X
//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public://�֐���`
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

protected://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

public://�ÓI�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device) { Fbx_Object3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx_Object3d::camera = camera; }

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Fbx_Model* fbx_model) { this->fbx_model = fbx_model; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	//getter
	const XMFLOAT3& GetScale() { return this->scale; }
	const XMFLOAT3& GetPosition() { return this->position; }

private://�ÓI�����o�ϐ�
	// �R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1, 1, 1 };
	//x, y, z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���[�J�����W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//���f��
	Fbx_Model* fbx_model = nullptr;
};

