#include "Tutorial.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "ParticleManager.h"
#include "Easing.h"
#include <fstream>

void Tutorial::Initialize(DirectXCommon* dxCommon)
{
	Audio* audio = Audio::GetInstance();


	//�J�����̏�����
	camera.reset(new FollowingCamera());
	//�J�����̏�����
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//���̃I�u�W�F�N�g����
	//floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	//floor->SetPosition({ 0,-18.5f,0 });

	//�f�[�^�ǂݍ���
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));


	//���@�̃I�u�W�F�N�g�Z�b�g+������
	player.reset(Player::Create(Model::CreateFromOBJ("Box")));
	player->SetPosition({ 0, 10.f, 0 });

	//�p�[�e�B�N�̃C���X�^���X
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//�V�F�[�_�[�̑}��
	postEffect->Initialize();

	//�}�b�v�`�b�v�ǂݍ��ݗp
	LoadFloorPopData();
}

void Tutorial::Finalize()
{
}

void Tutorial::Update()
{


	audio = Audio::GetInstance();
	if (!startFlag) {
		//���S���疾�邭����
		startEfRadius += 10.5f;
		if (startEfRadius >= 1000) {
			startFlag = true;
			startEfRadius -= 10.5f;
		}
	}
	Input* input = Input::GetInstance();

	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	nowCamera = camera.get();
	//�J������3D�I�u�W�F�b�g�ɃZ�b�g
	Object3d::SetCamera(nowCamera);

	camera->SetFollowingTarget(player.get());

	//if (rotateFlag) {
		//rotateTime--;
		////�Ǐ]�J�������畁�ʂ̃J�����ɕύX
		//nowCamera = debugCam.get();
		//Object3d::SetCamera(nowCamera);
		//nowCamera->SetEye({ player->GetPosition().x, 2.0f, 0 });
		//nowCamera->SetTarget(player->GetPosition());
		////��]������
		//playerRot = player->GetRotation();
		//playerRot.y += 3.0f;
		//player->SetRotation(playerRot);
		//if (rotateTime <= 0) {
		//	//�^�C����0�ɂȂ����玩�@����ɂ�����
		//	playerPos = player->GetPosition();
		//	if (easFrame < 1.0f) {
		//		easFrame += 0.01f;
		//		//���@�̑傫����������������֐�
		//		player->ScaleSmall();
		//	}

		//	playerPos.y = Ease(In, Cubic, easFrame, playerPos.y, 40.0f);
		//	playerScale = player->GetScale();
		//	//�v���C���[�̃X�P�[����0�ɂȂ�����V�[���ړ�
		//	if (playerScale.x <= 0.f && playerScale.y <= 0.f && playerScale.z <= 0.f) {
		//		BaseScene* scene = new GamePlayScene();
		//		this->sceneManager->SetNextScene(scene);
		//	}
		//	player->SetPosition(playerPos);
		//}
		//player->StopUpdate();
	//}


	//�v���C���[���X�e�[�W���痎������X�e�[�W�̐^�񒆂ɖ߂�
	if (player->GetPosition().y <= -20) {
		player->SetPosition({ 0,10,0 });
		player->SetRotation({ 0,0,0 });
	}

	//�e�I�u�W�F�N�g�̍X�V
	postEffect->SetRadius(startEfRadius);
	skyObj->Update();
	player->Update();
	UpdataFloorPopCommand();
	for (auto& floor : floors) {
		floor->Update();
	}
	camera->Update();
	debugCam->Update();

	//�����蔻��
	CheckAllCollision();
	particleMan->Update();
}

void Tutorial::Draw(DirectXCommon* dxCommon)
{
	//�`��O����
	//dxCommon->PreDraw();
	//�X�v���C�g�`��
#pragma region �w�i�X�v���C�g�`��
	postEffect->PreDrawScene(dxCommon->GetCmdList());

	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());
	//�w�i�X�v���C�g�`��

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion

#pragma endregion

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw();
	skyObj->Draw();
	player->Draw();
	for (auto& floor : floors) {
		floor->Draw();
	}

	if (!rotateFlag) {
		particleMan->Draw();
	}
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//�`��O����
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//�`��㏈��
	dxCommon->PostDraw();
}

void Tutorial::LoadFloorPopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/tutorial.csv");
	assert(file.is_open());

	//�t�@�C�����e�𕶎���X�g���[���ɃR�s�[
	floorPopCom << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void Tutorial::UpdataFloorPopCommand()
{
	//1�s���̕����������ϐ�
	std::string line;
	//�R�}���h���s���[�v
	while (getline(floorPopCom, line)) {
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//�R�����g�s�͔�΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0) {
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//�G�𔭐�������
			//�R���X�g���N�^�Ă�
			std::unique_ptr<Floor> newFloor = std::make_unique<Floor>();
			newFloor->Initialize(XMFLOAT3(x, y, z));
			//��Q����o�^����
			floors.push_back(std::move(newFloor));
		}
	}
}


void Tutorial::CheckAllCollision()
{
#pragma endregion

}
