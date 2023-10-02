#include "Framework.h"
#include "FbxLoader.h"
#include "Input.h"
#include "ParticleManager.h"
void Framework::Run()
{
	//�Q�[���̏�����
	Initialize();

	while (true)  // �Q�[�����[�v
	{
		//���t���[���X�V
		Update();
		Input* input = Input::GetInstance();
		if (input->PushKey(DIK_ESCAPE)) {
			break;
		}

		//�I�����N�G�X�g�������甲����
		if (IsEndRequst()) {
			break;
		}
		//�`��
		Draw();
	}
	// �Q�[���̏I��
	Finalize();
}

void Framework::Initialize()
{
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);


	// �X�v���C�g�ÓI������
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	ParticleManager::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList());

	//�f�o�b�N�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	const int debugTextTexNumber = 2;
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	//�f�o�b�N�e�L�X�g
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);


	//�I�[�f�B�I�̏�����
	audio = Audio::GetInstance();
	audio->Initialize();

	//3D�I�u�W�F�b�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(),dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
	//�V�[���}�l�[�M���[�̐���
	this->sceneManager = new SceneManager();
}

void Framework::Finalize()
{
	
	//�V�[���}�l�[�W���[���
	delete sceneManager;
	//�V���O���g���ɂ�����delete�_��
	//DirectX���
	delete dxCommon;
	//WindowsAPI�̏I������
	winApp->Finalize();
	//WindowsAPI���
	delete winApp;
	//FBX�̉��
	FbxLoader::GetInstance()->Finalize();

}

void Framework::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		//�Q�[�����[�v�𔲂���
		this->endRequst = true;
		return;
	}
	//���͂̍X�V
	input->Update();
	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	this->sceneManager->Update(dxCommon);
}

void Framework::Draw()
{
	//�V�[���`��
	this->sceneManager->Draw(dxCommon);
	//�f�o�b�N�e�L�X�g�`��
	//debugText->DrawAll();
}
