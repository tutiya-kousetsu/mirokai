#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//�Ō�̃V�[���̏I���Ɖ��
	this->scene->Finalize();
	delete this->scene;
}

void SceneManager::Update(DirectXCommon* dxCommon)
{
	//�V�[���̐؂�ւ�
	if (this->nextScene) {
		//���V�[���̏I��
		if (this->scene) {
			this->scene->Finalize();
			delete this->scene;
		}

		//�V�[���؂�ւ�
		this->scene = this->nextScene;
		this->nextScene = nullptr;


		//�V�[���}�l�[�W�����Z�b�g
		this->scene->SetSceneManager(this);
		//�V�V�[���̏�����
		this->scene->Initialize(dxCommon);
	}
	this->scene->Update();
}

void SceneManager::Draw(DirectXCommon* dxCommon)
{
	this->scene->Draw(dxCommon);
}
